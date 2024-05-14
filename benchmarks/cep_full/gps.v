//************************************************************************
// Copyright 2021 Massachusetts Institute of Technology
// SPDX short identifier: BSD-2-Clause
//
// File Name:      	gps.v
// Program:        	Common Evaluation Platform (CEP)
// Description:    	GPS top-level module
// Note: 			From the GPS transmitter's perspective
//
//************************************************************************
module gps(
           sys_clk_50,
           sync_rst_in,
           sv_num,
           startRound,
           aes_key,
           pcode_speeds,
           pcode_initializers,
           ca_code,
           p_code,
           l_code,
           l_code_valid
       );
input  sys_clk_50;
input  sync_rst_in;
input [5:0] sv_num;
input startRound;
input [191:0] aes_key;
input [30:0] pcode_speeds;
input [47:0] pcode_initializers;
output reg [12:0] ca_code;
output [127:0] p_code;
output [127:0] l_code;
output l_code_valid;


wire gps_clk_fast;

// Start of round edge detection
reg startRound_r;
always @(posedge gps_clk_fast)
    begin
        if(sync_rst_in) begin
            startRound_r <= 0;
        end else begin
            startRound_r <= startRound;
        end
    end
wire startRoundPosEdge = ~startRound_r & startRound;


//
// Tony Duong: Added logic to detect a change in *sv_num* number and ca-code and p-code calculation automatically
// to recompute CA-Code
//
reg [5:0] last_sv_num;
wire      sv_changed = last_sv_num != sv_num;
always @(posedge gps_clk_fast)
    begin
        if (sync_rst_in)
            last_sv_num <= 0;
        else
            last_sv_num <= sv_num;
    end


//
// Control code generators
//
reg p_gen_en;
reg ca_gen_en;
reg [7:0] p_bit_count;
reg [3:0] ca_bit_count;
//Get 128 P-Code bits and 13 CA-Code bits. (Real GPS is exactly a 10:1 ratio, this is close enough for here)
always @(posedge gps_clk_fast)
    begin
        if (sync_rst_in)
            p_gen_en <= 1'b0;
        else if(startRoundPosEdge) begin
            p_gen_en <= startRoundPosEdge;
        end else if(p_bit_count == 8'd127) begin //Stop 1 cycle early due to generator latency.
            p_gen_en <= 1'b0;
        end
    end

always @(posedge gps_clk_fast)
    begin
        if (sync_rst_in)
            ca_gen_en <= 1'b0;
        else if(startRoundPosEdge) begin
            ca_gen_en <= startRoundPosEdge;
        end else if(ca_bit_count == 4'd12) begin //Stop 1 cycle early due to generator latency.
            ca_gen_en <= 1'b0;
        end
    end


// Generate C/A code
wire ca_code_bit;
cacode ca(
          .clk         (gps_clk_slow ),
          .rst         (sync_rst_in  ),
          .prn_num     (sv_num       ),
          .prn_changed (sv_changed   ),
          .enb         (ca_gen_en    ),
          .chip_out    (ca_code_bit  )
         );

// Save 13 ca-code bits
always @(posedge gps_clk_slow)
    begin
        if(sync_rst_in) begin
            ca_bit_count <= 4'b0;
            ca_code  <= 13'b0;
        end else if (startRoundPosEdge) begin
            ca_bit_count <= 4'b0;
            ca_code  <= 13'b0;
        end else if(ca_bit_count < 4'd14) begin //Due to latency of CA generator, record 1 extra bit as first bit received is junk.
            ca_bit_count <= ca_bit_count + 1;
            ca_code <= {ca_code[11:0], ca_code_bit};
        end
    end

// Generate P-code
wire p_code_bit;
//verification connections:
wire [11:0] pcode_xn_cnt_speed; //default must be 1.
wire [18:0] pcode_z_cnt_speed; //default must be 1.
wire [11:0] pcode_ini_x1a;     //default must be 12'b001001001000;
wire [11:0] pcode_ini_x1b;     //default must be 12'b010101010100;
wire [11:0] pcode_ini_x2a;     //default must be 12'b100100100101;
wire [11:0] pcode_ini_x2b;     //default must be 12'b010101010100;
assign pcode_xn_cnt_speed = pcode_speeds[11:0];
assign pcode_z_cnt_speed  = pcode_speeds[30:12];
assign pcode_ini_x1a = pcode_initializers[11:0];
assign pcode_ini_x1b = pcode_initializers[23:12];
assign pcode_ini_x2a = pcode_initializers[35:24];
assign pcode_ini_x2b = pcode_initializers[47:36];
pcode p(
        .clk            (gps_clk_fast  ),
        .reset          (sync_rst_in   ),
        .prn_changed    (sv_changed    ),
        .en             (p_gen_en      ),
        .sat            (sv_num        ),
        .preg           (p_code_bit    ),
        .xn_cnt_speed   (pcode_xn_cnt_speed),
        .z_cnt_speed    (pcode_z_cnt_speed),
        .ini_x1a        (pcode_ini_x1a ),
        .ini_x1b        (pcode_ini_x1b ),
        .ini_x2a        (pcode_ini_x2a ),
        .ini_x2b        (pcode_ini_x2b )
      );

// Save 128 p-code bits, encrypt and send as l_code
reg [127:0] p_pt;
always @(posedge gps_clk_fast)
    begin
        if (sync_rst_in) begin
            p_bit_count <= 0;
            p_pt <= 0; 
        end else if(startRoundPosEdge) begin
            p_bit_count <= 0;
            p_pt <= 0;
        end else if ( p_gen_en) begin
            if(p_bit_count < 8'd128) begin
                p_bit_count <= p_bit_count + 1;
                p_pt <= {p_pt [126:0], p_code_bit};
            end
        end
    end


assign p_code = p_pt;

reg encrypt;
always @(posedge gps_clk_fast)
    begin
        if (sync_rst_in) 
            encrypt <= 0;
        else begin
            encrypt <= (p_bit_count == 8'd127); //Run encryption every 128th bit
        end
    end


//
// Todo : finish removing
assign gps_clk_fast = sys_clk_50;
assign gps_clk_slow = sys_clk_50;

//
// in order to remove the reset dependency, we need create a mask to mask out the out_valid from aes_192 module
// while computing CA-code and P-code, the valid is only applied when encryt=1
reg l_valid_mask;
wire l_code_valid_i;
assign l_code_valid = l_code_valid_i & l_valid_mask;
//
always @(posedge gps_clk_fast) begin
    if (sync_rst_in)            l_valid_mask <= 0;
    else if (startRoundPosEdge) l_valid_mask <= 0; // clear the mask
    else if (encrypt)           l_valid_mask <= 1; // this happens             
end


aes_192 aes_192_inst (
    .clk        (gps_clk_fast),
    .rst        (sync_rst_in),
    .start      (encrypt),
    .state      (p_pt),
    .key        (aes_key),
    .out        (l_code),
    .out_valid  (l_code_valid_i)
  );

endmodule

//************************************************************************
// Copyright 2021 Massachusetts Institute of Technology
// SPDX short identifier: BSD-2-Clause
//
// File Name:      	cacode.v
// Program:        	Common Evaluation Platform (CEP)
// Description:    	CA code generator
// Note: 			Generate 1023 chip sequence for SV PRN number prn_num.
//                  First chip on chip_out after reset.
//                  Asserting enb will advance to next chip.
//
//************************************************************************
module cacode
  (
   input       clk,
   input       rst,
   input [5:0] prn_num,
   input       prn_changed,
   input       enb,
   output reg  chip_out
   );

reg [10:1] g1, g2;
reg chip;

always @ (posedge clk) begin
    if (rst)
        begin
            g1 <= 10'b1111111111;
            g2 <= 10'b1111111111;
        end else begin
            if (prn_changed) begin
               g1 <= 10'b1111111111;
               g2 <= 10'b1111111111;
            end else if (enb) begin
                g1[10:1] <= {g1[9:1], g1[3] ^ g1[10]};
                g2[10:1] <= {g2[9:1], g2[2] ^ g2[3] ^ g2[6] ^ g2[8] ^ g2[9] ^ g2[10]};
            end
        end // else: !if(rst)
    end // always @ (posedge clk)

always @(*)
    begin
        if(rst)
           chip <= 1'b0; 
        else
    begin
        case (prn_num)
            6'd1 :
                chip <= g1[10] ^ g2[2] ^ g2[6];
            6'd2 :
                chip <= g1[10] ^ g2[3] ^ g2[7];
            6'd3 :
                chip <= g1[10] ^ g2[4] ^ g2[8];
            6'd4 :
                chip <= g1[10] ^ g2[5] ^ g2[9];
            6'd5 :
                chip <= g1[10] ^ g2[1] ^ g2[9];
            6'd6 :
                chip <= g1[10] ^ g2[2] ^ g2[10];
            6'd7 :
                chip <= g1[10] ^ g2[1] ^ g2[8];
            6'd8 :
                chip <= g1[10] ^ g2[2] ^ g2[9];
            6'd9 :
                chip <= g1[10] ^ g2[3] ^ g2[10];
            6'd10:
                chip <= g1[10] ^ g2[2] ^ g2[3];
            6'd11:
                chip <= g1[10] ^ g2[3] ^ g2[4];
            6'd12:
                chip <= g1[10] ^ g2[5] ^ g2[6];
            6'd13:
                chip <= g1[10] ^ g2[6] ^ g2[7];
            6'd14:
                chip <= g1[10] ^ g2[7] ^ g2[8];
            6'd15:
                chip <= g1[10] ^ g2[8] ^ g2[9];
            6'd16:
                chip <= g1[10] ^ g2[9] ^ g2[10];
            6'd17:
                chip <= g1[10] ^ g2[1] ^ g2[4];
            6'd18:
                chip <= g1[10] ^ g2[2] ^ g2[5];
            6'd19:
                chip <= g1[10] ^ g2[3] ^ g2[6];
            6'd20:
                chip <= g1[10] ^ g2[4] ^ g2[7];
            6'd21:
                chip <= g1[10] ^ g2[5] ^ g2[8];
            6'd22:
                chip <= g1[10] ^ g2[6] ^ g2[9];
            6'd23:
                chip <= g1[10] ^ g2[1] ^ g2[3];
            6'd24:
                chip <= g1[10] ^ g2[4] ^ g2[6];
            6'd25:
                chip <= g1[10] ^ g2[5] ^ g2[7];
            6'd26:
                chip <= g1[10] ^ g2[6] ^ g2[8];
            6'd27:
                chip <= g1[10] ^ g2[7] ^ g2[9];
            6'd28:
                chip <= g1[10] ^ g2[8] ^ g2[10];
            6'd29:
                chip <= g1[10] ^ g2[1] ^ g2[6];
            6'd30:
                chip <= g1[10] ^ g2[2] ^ g2[7];
            6'd31:
                chip <= g1[10] ^ g2[3] ^ g2[8];
            6'd32:
                chip <= g1[10] ^ g2[4] ^ g2[9];
            6'd33:
                chip <= g1[10] ^ g2[5] ^ g2[10];
            6'd34:
                chip <= g1[10] ^ g2[4] ^ g2[10];
            6'd35:
                chip <= g1[10] ^ g2[1] ^ g2[7];
            6'd36:
                chip <= g1[10] ^ g2[2] ^ g2[8];
            6'd37:
                chip <= g1[10] ^ g2[4] ^ g2[10];
            default:
                chip <= 1'b0;  //invalid prn_num
        endcase
    end
end


// reclock for timing
always @ (posedge clk)
    begin
        if(rst)
            chip_out<= 1'b0;
        else
            chip_out <= chip;
    end


endmodule

//************************************************************************
// Copyright 2021 Massachusetts Institute of Technology
// SPDX short identifier: BSD-2-Clause
//
// File Name:      pcode.v
// Program:        Common Evaluation Platform (CEP)
// Description:    P-code generator
//
//************************************************************************
module pcode(
           clk, reset, prn_changed, en, sat,
           preg, xn_cnt_speed, z_cnt_speed,
           ini_x1a, ini_x1b, ini_x2a, ini_x2b
       );
parameter SAT_WIDTH = 6;
parameter SREG_WIDTH = 37;
parameter XREG_WIDTH = 12;

input clk;
input reset;
input prn_changed;
input en;
input [ SAT_WIDTH-1:0] sat;
output wire preg; //Combinational output

//Verification-focused inputs
input [11:0] xn_cnt_speed; //default must be 1.
input [18:0] z_cnt_speed; //default must be 1.
input [11:0] ini_x1a;     //default must be 12'b001001001000;
input [11:0] ini_x1b;     //default must be 12'b010101010100;
input [11:0] ini_x2a;     //default must be 12'b100100100101;
input [11:0] ini_x2b;     //default must be 12'b010101010100;


wire rst = reset | prn_changed;
   
reg[XREG_WIDTH-1:0] x1a;
reg[XREG_WIDTH-1:0] x1b;
reg[XREG_WIDTH-1:0] x2a;
reg[XREG_WIDTH-1:0] x2b;
reg [SREG_WIDTH:0] sreg; //Note that sat is 1-based!

wire x1a_rst, x1b_rst, x2a_rst, x2b_rst;
wire x1a_cnt_d, x1b_cnt_d, x2a_cnt_d, x2b_cnt_d, x_cnt_d, z_cnt_last, z_cnt_eow, x1a_cnt_last;
reg[XREG_WIDTH-1:0] x1a_cnt, x1b_cnt, x2a_cnt, x2b_cnt;
reg[SAT_WIDTH-1:0] x_cnt;
reg[18:0] z_cnt;

wire x1b_en, x2a_en, x2b_en;
reg x1b_en_r, x2a_en_r, x2b_en_r;
wire x1b_res, x2a_res, x2b_res;
wire x1b_halt, x2a_halt, x2b_halt;

//////////////////////////////////////////
//Control Signals
//////////////////////////////////////////
assign x1b_res = x1a_cnt_d & x1a_rst;
assign x1b_halt= (x1b_cnt_d|x1a_cnt_last) & x1b_rst;

assign x2a_res = /*!x1a_cnt_d  &*/ (z_cnt_eow|x_cnt_d); //If on last cycle of X1A, do not continue
assign x2a_halt= (z_cnt_eow|x2a_cnt_d|x1a_cnt_last) & x2a_rst;

assign x2b_res = x2a_res;
assign x2b_halt= (z_cnt_eow|x2b_cnt_d|x1a_cnt_last) & x2b_rst;

assign z_cnt_last = (z_cnt >= 19'd403200-z_cnt_speed);
assign z_cnt_eow = (z_cnt_last & x1b_res);

assign x1a_cnt_last = x1a_cnt_d & z_cnt_last;

//////////////////////////////////////////
//Clock Control Signals
//////////////////////////////////////////
assign x1b_en = x1b_en_r & !x1b_halt;
assign x2a_en = x2a_en_r & !x2a_halt;
assign x2b_en = x2b_en_r & !x2b_halt;

always @(posedge clk)
    begin
        if(rst)
            x1b_en_r<=1;
        else if(en) begin
            if(x1b_res)
                x1b_en_r<=1;
            else if(x1b_halt)
                x1b_en_r<=0;
        end
    end

always @(posedge clk)
    begin
        if(rst)
            x2a_en_r<=1;
        else if(en) begin
            if(x2a_res)
                x2a_en_r<=1;
            else if(x2a_halt)
                x2a_en_r<=0;
        end
    end

always @(posedge clk)
    begin
        if(rst)
            x2b_en_r<=1;
        else if(en) begin
            if(x2b_res)
                x2b_en_r<=1;
            else if(x2b_halt)
                x2b_en_r<=0;
        end
    end

//////////////////////////////////////////
//Decoders
//////////////////////////////////////////
assign x1a_rst = (x1a==12'b000100100100); //4092nd state
assign x1b_rst = (x1b==12'b001010101010); //4093rd state
assign x2a_rst = (x2a==12'b110010010010); //4092nd state
assign x2b_rst = (x2b==12'b001010101010); //4093rd state

//////////////////////////////////////////
//Counters
//////////////////////////////////////////
assign x1a_cnt_d = (x1a_cnt>=12'd3750-xn_cnt_speed); //Comparison instead of equality due to configurable counter increment
assign x1b_cnt_d = (x1b_cnt>=12'd3749-xn_cnt_speed);
assign x2a_cnt_d = (x2a_cnt>=12'd3750-xn_cnt_speed);
assign x2b_cnt_d = (x2b_cnt>=12'd3749-xn_cnt_speed);
assign x_cnt_d   = (x_cnt  ==6'd37);

always @(posedge clk)
    begin
        if(rst)
            x1a_cnt  <=0;
        else if(en & x1a_rst) begin
            if (!x1a_cnt_d)
                x1a_cnt <= x1a_cnt+xn_cnt_speed;
            else
                x1a_cnt <= 12'd0;
        end
    end

always @(posedge clk)
    begin
        if(rst)
            x1b_cnt <=0;
        else if(en & x1b_rst & x1b_en_r) begin //TODO: x1b_en_r?
            if (!x1b_cnt_d & !x1a_cnt_last)
                x1b_cnt <= x1b_cnt+xn_cnt_speed;
            else
                x1b_cnt <= 12'd0;
        end
    end

always @(posedge clk)
    begin
        if(rst)
            x2a_cnt<=0;
        else if(en & x2a_rst & x2a_en_r) begin
            if (!x2a_cnt_d & !x1a_cnt_last)
                x2a_cnt <= x2a_cnt+xn_cnt_speed;
            else
                x2a_cnt <=12'd0;
        end
    end

always @(posedge clk)
    begin
        if(rst)
            x2b_cnt  <=0;
        else if(en & x2b_rst & x2b_en_r) begin
            if (!x2b_cnt_d & !x1a_cnt_last)
                x2b_cnt <= x2b_cnt+xn_cnt_speed;
            else
                x2b_cnt <=12'd0;
        end
    end

always @(posedge clk)
    begin
        if(rst)
            x_cnt<=0;
        else if(en & ((x2a_rst & x2a_cnt_d) | (x_cnt!=0))) begin
            if(x_cnt < 6'd37)
                x_cnt <= x_cnt+1;
            else
                x_cnt <= 6'd0;
        end
    end

always @(posedge clk)
    begin
        if(rst)
            z_cnt<=0;
        else if(en & x1b_res) begin
            if (!z_cnt_last)
                z_cnt <= z_cnt+z_cnt_speed;
            else
                z_cnt <=19'd0;
        end
    end

//////////////////////////////////////////
//4 shift registers
//////////////////////////////////////////
always @(posedge clk)
    begin
        if(rst|x1a_rst)
            x1a<=ini_x1a;
        else if(en)
            x1a<={x1a[XREG_WIDTH-2:0],x1a[5]^x1a[7]^x1a[10]^x1a[11]};
    end

always @(posedge clk)
    begin
        //If enabled, handle resume/reset immediately.
        //Keep in mind that *_en update is delayed 1 cycle from *_halt, but need to halt immediately.
        if(rst | (en & (x1b_res | (x1b_rst & x1b_en))))
            x1b<=ini_x1b;
        else if (en & x1b_en)
            x1b<={x1b[XREG_WIDTH-2:0],x1b[0]^x1b[1]^x1b[4]^x1b[7]^x1b[8]^x1b[9]^x1b[10]^x1b[11]};
    end

always @(posedge clk)
    begin
        if(rst | (en & (x2a_res | (x2a_rst & x2a_en))))
            x2a<=ini_x2a;
        else if (en & x2a_en)
            x2a<={x2a[XREG_WIDTH-2:0],x2a[0]^x2a[2]^x2a[3]^x2a[4]^x2a[6]^x2a[7]^x2a[8]^x2a[9]^x2a[10]^x2a[11]};
    end

always @(posedge clk)
    begin
        if (rst | (en & (x2b_res | (x2b_rst & x2b_en))))
            x2b<=ini_x2b;
        else if (en & x2b_en)
            x2b<={x2b[XREG_WIDTH-2:0],x2b[1]^x2b[2]^x2b[3]^x2b[7]^x2b[8]^x2b[11]};
    end


//x2 shift register
always @(posedge clk)
    begin
        if(rst)
            sreg<={SREG_WIDTH{1'b1}};
        else if(en)
            sreg<={sreg[SREG_WIDTH-1:0], (x2a[XREG_WIDTH-1]^x2b[XREG_WIDTH-1])};
    end


//Output
assign preg = (rst | (sat==0)) ? 1'b0 : (x1a[XREG_WIDTH-1]^x1b[XREG_WIDTH-1]) ^ sreg[sat-1]; 


endmodule


/*
 * Copyright 2012, Homer Hsing <homer.hsing@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


// This is a fully unrolled implementation
module aes_192 (clk, rst, start, state, key, out, out_valid);
    input wire          clk; 
    input wire          rst; 
    input wire          start;
    input wire [127:0]  state;
    input wire [191:0]  key;
    output wire [127:0] out;
    output wire         out_valid;

    // Internals signals and such
    reg    [127:0]  s0;
    reg    [191:0]  k0;
    wire   [127:0]  s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    wire   [191:0]  k1, k2, k3, k4, k5, k6, k7, k8, k9, k10, k11;
    wire   [127:0]  k0b, k1b, k2b, k3b, k4b, k5b, k6b, k7b, k8b, k9b, k10b, k11b;
    reg             start_r;
    wire            start_posedge;
    reg    [4:0]    validCounter;
    
    assign start_posedge = start & ~start_r;

    always @(posedge clk)
    begin
        if (rst)
            start_r <= 1'b0;
        else
            start_r <= start;
    end // end always

    always @ (posedge clk)
    begin
        if (rst) begin
            s0              <= 0;
            k0              <= 0;
            validCounter    <= 0;
        end else
            if(start_posedge)
                begin
                    s0 <= state ^ key[191:64];
                    k0 <= key;
                    validCounter <= 26;
                end
            else if(validCounter > 1)
                begin
                    validCounter <= validCounter - 1;
                end
    end // end always

    assign out_valid = (validCounter == 1);
    
    expand_key_type_D_192  a0 (clk, rst, k0, 8'h1,   k1,  k0b);
    expand_key_type_B_192  a1 (clk, rst, k1,         k2,  k1b);
    expand_key_type_A_192  a2 (clk, rst, k2, 8'h2,   k3,  k2b);
    expand_key_type_C_192  a3 (clk, rst, k3, 8'h4,   k4,  k3b);
    expand_key_type_B_192  a4 (clk, rst, k4,         k5,  k4b);
    expand_key_type_A_192  a5 (clk, rst, k5, 8'h8,   k6,  k5b);
    expand_key_type_C_192  a6 (clk, rst, k6, 8'h10,  k7,  k6b);
    expand_key_type_B_192  a7 (clk, rst, k7,         k8,  k7b);
    expand_key_type_A_192  a8 (clk, rst, k8, 8'h20,  k9,  k8b);
    expand_key_type_C_192  a9 (clk, rst, k9, 8'h40,  k10, k9b);
    expand_key_type_B_192 a10 (clk, rst, k10,        k11, k10b);
    expand_key_type_A_192 a11 (clk, rst, k11, 8'h80,    , k11b);

    one_round
        r1 (clk, rst, s0, k0b, s1),
        r2 (clk, rst, s1, k1b, s2),
        r3 (clk, rst, s2, k2b, s3),
        r4 (clk, rst, s3, k3b, s4),
        r5 (clk, rst, s4, k4b, s5),
        r6 (clk, rst, s5, k5b, s6),
        r7 (clk, rst, s6, k6b, s7),
        r8 (clk, rst, s7, k7b, s8),
        r9 (clk, rst, s8, k8b, s9),
        r10 (clk, rst, s9, k9b, s10),
        r11 (clk, rst, s10, k10b, s11);

    final_round
        rf (clk, rst, s11, k11b, out);

endmodule

/* expand k0,k1,k2,k3 for every two clock cycles */
module expand_key_type_A_192 (clk, rst, in, rcon, out_1, out_2);
    input wire          clk;
    input wire          rst;
    input wire  [191:0] in;
    input wire  [7:0]   rcon;
    output reg  [191:0] out_1;
    output wire [127:0] out_2;

    // Internal signals
    wire       [31:0]  k0, k1, k2, k3, k4, k5, v0, v1, v2, v3;
    reg        [31:0]  k0a, k1a, k2a, k3a, k4a, k5a;
    wire       [31:0]  k0b, k1b, k2b, k3b, k4b, k5b, k6a;

    assign {k0, k1, k2, k3, k4, k5} = in;

    assign v0 = {k0[31:24] ^ rcon, k0[23:0]};
    assign v1 = v0 ^ k1;
    assign v2 = v1 ^ k2;
    assign v3 = v2 ^ k3;

    always @ (posedge clk) 
    begin
        if (rst)
            {k0a, k1a, k2a, k3a, k4a, k5a} <= {32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd0};
        else
            {k0a, k1a, k2a, k3a, k4a, k5a} <= {v0, v1, v2, v3, k4, k5};
    end // end always

    S4 S4_0 (clk, rst, {k5[23:0], k5[31:24]}, k6a);

    assign k0b = k0a ^ k6a;
    assign k1b = k1a ^ k6a;
    assign k2b = k2a ^ k6a;
    assign k3b = k3a ^ k6a;
    assign {k4b, k5b} = {k4a, k5a};

    always @ (posedge clk)
    begin
        if (rst)
            out_1 <= 0;
        else
            out_1 <= {k0b, k1b, k2b, k3b, k4b, k5b};
    end // end always

    assign out_2 = {k0b, k1b, k2b, k3b};

endmodule   // end module expand_key_type_A_192


/* expand k2,k3,k4,k5 for every two clock cycles */
module expand_key_type_B_192 (clk, rst, in, out_1, out_2);
    input wire          clk;
    input wire          rst;
    input wire  [191:0] in;
    output reg  [191:0] out_1;
    output wire [127:0] out_2;
    wire        [31:0]  k0, k1, k2, k3, k4, k5, v2, v3, v4, v5;
    reg         [31:0]  k0a, k1a, k2a, k3a, k4a, k5a;

    assign {k0, k1, k2, k3, k4, k5} = in;

    assign v2 = k1 ^ k2;
    assign v3 = v2 ^ k3;
    assign v4 = v3 ^ k4;
    assign v5 = v4 ^ k5;

    always @ (posedge clk)
    begin
        if (rst)
            {k0a, k1a, k2a, k3a, k4a, k5a} <= {32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd0};
        else
            {k0a, k1a, k2a, k3a, k4a, k5a} <= {k0, k1, v2, v3, v4, v5};
    end // end always

    always @ (posedge clk)
    begin
        if (rst)
            out_1   <= 0;
        else
            out_1   <= {k0a, k1a, k2a, k3a, k4a, k5a};
    end

    assign out_2 = {k2a, k3a, k4a, k5a};

endmodule   // end expand_key_type_B_192



/* expand k0,k1,k4,k5 for every two clock cycles */
module expand_key_type_C_192 (clk, rst, in, rcon, out_1, out_2);
    input wire          clk;
    input wire          rst;
    input wire  [191:0] in;
    input wire  [7:0]   rcon;
    output reg  [191:0] out_1;
    output wire [127:0] out_2;

    wire       [31:0]  k0, k1, k2, k3, k4, k5, v4, v5, v0, v1;
    reg        [31:0]  k0a, k1a, k2a, k3a, k4a, k5a;
    wire       [31:0]  k0b, k1b, k2b, k3b, k4b, k5b, k6a;

    assign {k0, k1, k2, k3, k4, k5} = in;
    
    assign v4 = k3 ^ k4;
    assign v5 = v4 ^ k5;
    assign v0 = {k0[31:24] ^ rcon, k0[23:0]};
    assign v1 = v0 ^ k1;

    always @ (posedge clk)
    begin
        if (rst)
            {k0a, k1a, k2a, k3a, k4a, k5a} <= {32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd0};
        else
            {k0a, k1a, k2a, k3a, k4a, k5a} <= {v0, v1, k2, k3, v4, v5};
    end

    S4 S4_0 (clk, rst, {v5[23:0], v5[31:24]}, k6a);

    assign k0b = k0a ^ k6a;
    assign k1b = k1a ^ k6a;
    assign {k2b, k3b, k4b, k5b} = {k2a, k3a, k4a, k5a};

    always @ (posedge clk)
    begin
        if (rst)
            out_1   <= 0;
        else
            out_1   <= {k0b, k1b, k2b, k3b, k4b, k5b};
    end

    assign out_2 = {k4b, k5b, k0b, k1b};

endmodule   // end expand_key_type_C_192

/* expand k0,k1 for every two clock cycles */
module expand_key_type_D_192 (clk, rst, in, rcon, out_1, out_2);
    input wire          clk;
    input wire          rst;
    input wire  [191:0] in;
    input wire  [7:0]   rcon;
    output reg  [191:0] out_1;
    output wire [127:0] out_2;
    wire       [31:0]  k0, k1, k2, k3, k4, k5, v0, v1;
    reg        [31:0]  k0a, k1a, k2a, k3a, k4a, k5a;
    wire       [31:0]  k0b, k1b, k2b, k3b, k4b, k5b, k6a;

    assign {k0, k1, k2, k3, k4, k5} = in;

    assign v0 = {k0[31:24] ^ rcon, k0[23:0]};
    assign v1 = v0 ^ k1;

    always @ (posedge clk)
    begin
        if (rst)
            {k0a, k1a, k2a, k3a, k4a, k5a}  <= {32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd0};
        else
            {k0a, k1a, k2a, k3a, k4a, k5a}  <= {v0, v1, k2, k3, k4, k5};
    end // end always

    S4 S4_0 (clk, rst, {k5[23:0], k5[31:24]}, k6a);

    assign k0b = k0a ^ k6a;
    assign k1b = k1a ^ k6a;
    assign {k2b, k3b, k4b, k5b} = {k2a, k3a, k4a, k5a};

    always @ (posedge clk)
    begin
        if (rst)
            out_1   <= 0;
        else
            out_1   <= {k0b, k1b, k2b, k3b, k4b, k5b};
    end // end always

    assign out_2 = {k4b, k5b, k0b, k1b};

endmodule   // end expand_key_type_D_192   


/*
 * Copyright 2012, Homer Hsing <homer.hsing@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* verilator lint_off UNOPTFLAT */

/* one AES round for every two clock cycles */
module one_round (clk, rst, state_in, key, state_out);
input              clk;
input              rst;
input      [127:0] state_in, key;
output reg [127:0] state_out;
wire       [31:0]  s0,  s1,  s2,  s3,
           z0,  z1,  z2,  z3,
           p00, p01, p02, p03,
           p10, p11, p12, p13,
           p20, p21, p22, p23,
           p30, p31, p32, p33,
           k0,  k1,  k2,  k3;

assign {k0, k1, k2, k3} = key;

assign {s0, s1, s2, s3} = state_in;

table_lookup
    t0 (clk, rst, s0, p00, p01, p02, p03),
    t1 (clk, rst, s1, p10, p11, p12, p13),
    t2 (clk, rst, s2, p20, p21, p22, p23),
    t3 (clk, rst, s3, p30, p31, p32, p33);

assign z0 = p00 ^ p11 ^ p22 ^ p33 ^ k0;
assign z1 = p03 ^ p10 ^ p21 ^ p32 ^ k1;
assign z2 = p02 ^ p13 ^ p20 ^ p31 ^ k2;
assign z3 = p01 ^ p12 ^ p23 ^ p30 ^ k3;

always @ (posedge clk)
    state_out <= {z0, z1, z2, z3};
endmodule

    /* AES final round for every two clock cycles */
    module final_round (clk, rst, state_in, key_in, state_out);
input              clk;
input              rst;
input      [127:0] state_in;
input      [127:0] key_in;
output reg [127:0] state_out;
wire [31:0] s0,  s1,  s2,  s3,
     z0,  z1,  z2,  z3,
     k0,  k1,  k2,  k3;
wire [7:0]  p00, p01, p02, p03,
     p10, p11, p12, p13,
     p20, p21, p22, p23,
     p30, p31, p32, p33;

assign {k0, k1, k2, k3} = key_in;

assign {s0, s1, s2, s3} = state_in;

S4
    S4_1 (clk, rst, s0, {p00, p01, p02, p03}),
    S4_2 (clk, rst, s1, {p10, p11, p12, p13}),
    S4_3 (clk, rst, s2, {p20, p21, p22, p23}),
    S4_4 (clk, rst, s3, {p30, p31, p32, p33});

assign z0 = {p00, p11, p22, p33} ^ k0;
assign z1 = {p10, p21, p32, p03} ^ k1;
assign z2 = {p20, p31, p02, p13} ^ k2;
assign z3 = {p30, p01, p12, p23} ^ k3;

always @ (posedge clk)
    if (rst)
      state_out <= 0;
    else
      state_out <= {z0, z1, z2, z3};
endmodule



/*
 * Copyright 2012, Homer Hsing <homer.hsing@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* verilator lint_off UNOPTFLAT */

module table_lookup (clk, rst, state, p0, p1, p2, p3);
    input clk;
    input rst;
    input [31:0] state;
    output [31:0] p0, p1, p2, p3;
    
    wire [7:0] b0, b1, b2, b3;

    assign {b0, b1, b2, b3} = state;
    T
        t0 (clk, rst, b0, {p0[23:0], p0[31:24]}),
        t1 (clk, rst, b1, {p1[15:0], p1[31:16]}),
        t2 (clk, rst, b2, {p2[7:0],  p2[31:8]} ),
        t3 (clk, rst, b3, p3);
endmodule   // end table_lookup

/* substitue four bytes in a word */
module S4 (clk, rst, in, out);
    input clk;
    input rst;
    input [31:0] in;
    output [31:0] out;

    S
        S_0 (clk, rst, in[31:24], out[31:24]),
        S_1 (clk, rst, in[23:16], out[23:16]),
        S_2 (clk, rst, in[15:8],  out[15:8] ),
        S_3 (clk, rst, in[7:0],   out[7:0]  );

endmodule   // end S4

/* S_box, S_box, S_box*(x+1), S_box*x */
module T (clk, rst, in, out);
    input         clk;
    input         rst;
    input  [7:0]  in;
    output [31:0] out;

    S
        s0 (clk, rst, in, out[31:24]);

    assign out[23:16] = out[31:24];

    xS
        s4 (clk, rst, in, out[7:0]);

    assign out[15:8] = out[23:16] ^ out[7:0];

endmodule   // end T

/* S box */
module S (clk, rst, in, out);
    input clk;
    input rst;
    input [7:0] in;
    output reg [7:0] out;

    always @ (posedge clk)
        if (rst)
            out <= 8'd0;
        else
            case (in)
            8'h00:  out <= 8'h63;
            8'h01:  out <= 8'h7c;
            8'h02:  out <= 8'h77;
            8'h03:  out <= 8'h7b;
            8'h04:  out <= 8'hf2;
            8'h05:  out <= 8'h6b;
            8'h06:  out <= 8'h6f;
            8'h07:  out <= 8'hc5;
            8'h08:  out <= 8'h30;
            8'h09:  out <= 8'h01;
            8'h0a:  out <= 8'h67;
            8'h0b:  out <= 8'h2b;
            8'h0c:  out <= 8'hfe;
            8'h0d:  out <= 8'hd7;
            8'h0e:  out <= 8'hab;
            8'h0f:  out <= 8'h76;
            8'h10:  out <= 8'hca;
            8'h11:  out <= 8'h82;
            8'h12:  out <= 8'hc9;
            8'h13:  out <= 8'h7d;
            8'h14:  out <= 8'hfa;
            8'h15:  out <= 8'h59;
            8'h16:  out <= 8'h47;
            8'h17:  out <= 8'hf0;
            8'h18:  out <= 8'had;
            8'h19:  out <= 8'hd4;
            8'h1a:  out <= 8'ha2;
            8'h1b:  out <= 8'haf;
            8'h1c:  out <= 8'h9c;
            8'h1d:  out <= 8'ha4;
            8'h1e:  out <= 8'h72;
            8'h1f:  out <= 8'hc0;
            8'h20:  out <= 8'hb7;
            8'h21:  out <= 8'hfd;
            8'h22:  out <= 8'h93;
            8'h23:  out <= 8'h26;
            8'h24:  out <= 8'h36;
            8'h25:  out <= 8'h3f;
            8'h26:  out <= 8'hf7;
            8'h27:  out <= 8'hcc;
            8'h28:  out <= 8'h34;
            8'h29:  out <= 8'ha5;
            8'h2a:  out <= 8'he5;
            8'h2b:  out <= 8'hf1;
            8'h2c:  out <= 8'h71;
            8'h2d:  out <= 8'hd8;
            8'h2e:  out <= 8'h31;
            8'h2f:  out <= 8'h15;
            8'h30:  out <= 8'h04;
            8'h31:  out <= 8'hc7;
            8'h32:  out <= 8'h23;
            8'h33:  out <= 8'hc3;
            8'h34:  out <= 8'h18;
            8'h35:  out <= 8'h96;
            8'h36:  out <= 8'h05;
            8'h37:  out <= 8'h9a;
            8'h38:  out <= 8'h07;
            8'h39:  out <= 8'h12;
            8'h3a:  out <= 8'h80;
            8'h3b:  out <= 8'he2;
            8'h3c:  out <= 8'heb;
            8'h3d:  out <= 8'h27;
            8'h3e:  out <= 8'hb2;
            8'h3f:  out <= 8'h75;
            8'h40:  out <= 8'h09;
            8'h41:  out <= 8'h83;
            8'h42:  out <= 8'h2c;
            8'h43:  out <= 8'h1a;
            8'h44:  out <= 8'h1b;
            8'h45:  out <= 8'h6e;
            8'h46:  out <= 8'h5a;
            8'h47:  out <= 8'ha0;
            8'h48:  out <= 8'h52;
            8'h49:  out <= 8'h3b;
            8'h4a:  out <= 8'hd6;
            8'h4b:  out <= 8'hb3;
            8'h4c:  out <= 8'h29;
            8'h4d:  out <= 8'he3;
            8'h4e:  out <= 8'h2f;
            8'h4f:  out <= 8'h84;
            8'h50:  out <= 8'h53;
            8'h51:  out <= 8'hd1;
            8'h52:  out <= 8'h00;
            8'h53:  out <= 8'hed;
            8'h54:  out <= 8'h20;
            8'h55:  out <= 8'hfc;
            8'h56:  out <= 8'hb1;
            8'h57:  out <= 8'h5b;
            8'h58:  out <= 8'h6a;
            8'h59:  out <= 8'hcb;
            8'h5a:  out <= 8'hbe;
            8'h5b:  out <= 8'h39;
            8'h5c:  out <= 8'h4a;
            8'h5d:  out <= 8'h4c;
            8'h5e:  out <= 8'h58;
            8'h5f:  out <= 8'hcf;
            8'h60:  out <= 8'hd0;
            8'h61:  out <= 8'hef;
            8'h62:  out <= 8'haa;
            8'h63:  out <= 8'hfb;
            8'h64:  out <= 8'h43;
            8'h65:  out <= 8'h4d;
            8'h66:  out <= 8'h33;
            8'h67:  out <= 8'h85;
            8'h68:  out <= 8'h45;
            8'h69:  out <= 8'hf9;
            8'h6a:  out <= 8'h02;
            8'h6b:  out <= 8'h7f;
            8'h6c:  out <= 8'h50;
            8'h6d:  out <= 8'h3c;
            8'h6e:  out <= 8'h9f;
            8'h6f:  out <= 8'ha8;
            8'h70:  out <= 8'h51;
            8'h71:  out <= 8'ha3;
            8'h72:  out <= 8'h40;
            8'h73:  out <= 8'h8f;
            8'h74:  out <= 8'h92;
            8'h75:  out <= 8'h9d;
            8'h76:  out <= 8'h38;
            8'h77:  out <= 8'hf5;
            8'h78:  out <= 8'hbc;
            8'h79:  out <= 8'hb6;
            8'h7a:  out <= 8'hda;
            8'h7b:  out <= 8'h21;
            8'h7c:  out <= 8'h10;
            8'h7d:  out <= 8'hff;
            8'h7e:  out <= 8'hf3;
            8'h7f:  out <= 8'hd2;
            8'h80:  out <= 8'hcd;
            8'h81:  out <= 8'h0c;
            8'h82:  out <= 8'h13;
            8'h83:  out <= 8'hec;
            8'h84:  out <= 8'h5f;
            8'h85:  out <= 8'h97;
            8'h86:  out <= 8'h44;
            8'h87:  out <= 8'h17;
            8'h88:  out <= 8'hc4;
            8'h89:  out <= 8'ha7;
            8'h8a:  out <= 8'h7e;
            8'h8b:  out <= 8'h3d;
            8'h8c:  out <= 8'h64;
            8'h8d:  out <= 8'h5d;
            8'h8e:  out <= 8'h19;
            8'h8f:  out <= 8'h73;
            8'h90:  out <= 8'h60;
            8'h91:  out <= 8'h81;
            8'h92:  out <= 8'h4f;
            8'h93:  out <= 8'hdc;
            8'h94:  out <= 8'h22;
            8'h95:  out <= 8'h2a;
            8'h96:  out <= 8'h90;
            8'h97:  out <= 8'h88;
            8'h98:  out <= 8'h46;
            8'h99:  out <= 8'hee;
            8'h9a:  out <= 8'hb8;
            8'h9b:  out <= 8'h14;
            8'h9c:  out <= 8'hde;
            8'h9d:  out <= 8'h5e;
            8'h9e:  out <= 8'h0b;
            8'h9f:  out <= 8'hdb;
            8'ha0:  out <= 8'he0;
            8'ha1:  out <= 8'h32;
            8'ha2:  out <= 8'h3a;
            8'ha3:  out <= 8'h0a;
            8'ha4:  out <= 8'h49;
            8'ha5:  out <= 8'h06;
            8'ha6:  out <= 8'h24;
            8'ha7:  out <= 8'h5c;
            8'ha8:  out <= 8'hc2;
            8'ha9:  out <= 8'hd3;
            8'haa:  out <= 8'hac;
            8'hab:  out <= 8'h62;
            8'hac:  out <= 8'h91;
            8'had:  out <= 8'h95;
            8'hae:  out <= 8'he4;
            8'haf:  out <= 8'h79;
            8'hb0:  out <= 8'he7;
            8'hb1:  out <= 8'hc8;
            8'hb2:  out <= 8'h37;
            8'hb3:  out <= 8'h6d;
            8'hb4:  out <= 8'h8d;
            8'hb5:  out <= 8'hd5;
            8'hb6:  out <= 8'h4e;
            8'hb7:  out <= 8'ha9;
            8'hb8:  out <= 8'h6c;
            8'hb9:  out <= 8'h56;
            8'hba:  out <= 8'hf4;
            8'hbb:  out <= 8'hea;
            8'hbc:  out <= 8'h65;
            8'hbd:  out <= 8'h7a;
            8'hbe:  out <= 8'hae;
            8'hbf:  out <= 8'h08;
            8'hc0:  out <= 8'hba;
            8'hc1:  out <= 8'h78;
            8'hc2:  out <= 8'h25;
            8'hc3:  out <= 8'h2e;
            8'hc4:  out <= 8'h1c;
            8'hc5:  out <= 8'ha6;
            8'hc6:  out <= 8'hb4;
            8'hc7:  out <= 8'hc6;
            8'hc8:  out <= 8'he8;
            8'hc9:  out <= 8'hdd;
            8'hca:  out <= 8'h74;
            8'hcb:  out <= 8'h1f;
            8'hcc:  out <= 8'h4b;
            8'hcd:  out <= 8'hbd;
            8'hce:  out <= 8'h8b;
            8'hcf:  out <= 8'h8a;
            8'hd0:  out <= 8'h70;
            8'hd1:  out <= 8'h3e;
            8'hd2:  out <= 8'hb5;
            8'hd3:  out <= 8'h66;
            8'hd4:  out <= 8'h48;
            8'hd5:  out <= 8'h03;
            8'hd6:  out <= 8'hf6;
            8'hd7:  out <= 8'h0e;
            8'hd8:  out <= 8'h61;
            8'hd9:  out <= 8'h35;
            8'hda:  out <= 8'h57;
            8'hdb:  out <= 8'hb9;
            8'hdc:  out <= 8'h86;
            8'hdd:  out <= 8'hc1;
            8'hde:  out <= 8'h1d;
            8'hdf:  out <= 8'h9e;
            8'he0:  out <= 8'he1;
            8'he1:  out <= 8'hf8;
            8'he2:  out <= 8'h98;
            8'he3:  out <= 8'h11;
            8'he4:  out <= 8'h69;
            8'he5:  out <= 8'hd9;
            8'he6:  out <= 8'h8e;
            8'he7:  out <= 8'h94;
            8'he8:  out <= 8'h9b;
            8'he9:  out <= 8'h1e;
            8'hea:  out <= 8'h87;
            8'heb:  out <= 8'he9;
            8'hec:  out <= 8'hce;
            8'hed:  out <= 8'h55;
            8'hee:  out <= 8'h28;
            8'hef:  out <= 8'hdf;
            8'hf0:  out <= 8'h8c;
            8'hf1:  out <= 8'ha1;
            8'hf2:  out <= 8'h89;
            8'hf3:  out <= 8'h0d;
            8'hf4:  out <= 8'hbf;
            8'hf5:  out <= 8'he6;
            8'hf6:  out <= 8'h42;
            8'hf7:  out <= 8'h68;
            8'hf8:  out <= 8'h41;
            8'hf9:  out <= 8'h99;
            8'hfa:  out <= 8'h2d;
            8'hfb:  out <= 8'h0f;
            8'hfc:  out <= 8'hb0;
            8'hfd:  out <= 8'h54;
            8'hfe:  out <= 8'hbb;
            8'hff:  out <= 8'h16;
        endcase
endmodule

/* S box * x */
module xS (clk, rst, in, out);
    input clk;
    input rst;
    input [7:0] in;
    output reg [7:0] out;

    always @ (posedge clk)
        if (rst)
            out <= 8'd0;
        else
            case (in)
            8'h00:  out <= 8'hc6;
            8'h01:  out <= 8'hf8;
            8'h02:  out <= 8'hee;
            8'h03:  out <= 8'hf6;
            8'h04:  out <= 8'hff;
            8'h05:  out <= 8'hd6;
            8'h06:  out <= 8'hde;
            8'h07:  out <= 8'h91;
            8'h08:  out <= 8'h60;
            8'h09:  out <= 8'h02;
            8'h0a:  out <= 8'hce;
            8'h0b:  out <= 8'h56;
            8'h0c:  out <= 8'he7;
            8'h0d:  out <= 8'hb5;
            8'h0e:  out <= 8'h4d;
            8'h0f:  out <= 8'hec;
            8'h10:  out <= 8'h8f;
            8'h11:  out <= 8'h1f;
            8'h12:  out <= 8'h89;
            8'h13:  out <= 8'hfa;
            8'h14:  out <= 8'hef;
            8'h15:  out <= 8'hb2;
            8'h16:  out <= 8'h8e;
            8'h17:  out <= 8'hfb;
            8'h18:  out <= 8'h41;
            8'h19:  out <= 8'hb3;
            8'h1a:  out <= 8'h5f;
            8'h1b:  out <= 8'h45;
            8'h1c:  out <= 8'h23;
            8'h1d:  out <= 8'h53;
            8'h1e:  out <= 8'he4;
            8'h1f:  out <= 8'h9b;
            8'h20:  out <= 8'h75;
            8'h21:  out <= 8'he1;
            8'h22:  out <= 8'h3d;
            8'h23:  out <= 8'h4c;
            8'h24:  out <= 8'h6c;
            8'h25:  out <= 8'h7e;
            8'h26:  out <= 8'hf5;
            8'h27:  out <= 8'h83;
            8'h28:  out <= 8'h68;
            8'h29:  out <= 8'h51;
            8'h2a:  out <= 8'hd1;
            8'h2b:  out <= 8'hf9;
            8'h2c:  out <= 8'he2;
            8'h2d:  out <= 8'hab;
            8'h2e:  out <= 8'h62;
            8'h2f:  out <= 8'h2a;
            8'h30:  out <= 8'h08;
            8'h31:  out <= 8'h95;
            8'h32:  out <= 8'h46;
            8'h33:  out <= 8'h9d;
            8'h34:  out <= 8'h30;
            8'h35:  out <= 8'h37;
            8'h36:  out <= 8'h0a;
            8'h37:  out <= 8'h2f;
            8'h38:  out <= 8'h0e;
            8'h39:  out <= 8'h24;
            8'h3a:  out <= 8'h1b;
            8'h3b:  out <= 8'hdf;
            8'h3c:  out <= 8'hcd;
            8'h3d:  out <= 8'h4e;
            8'h3e:  out <= 8'h7f;
            8'h3f:  out <= 8'hea;
            8'h40:  out <= 8'h12;
            8'h41:  out <= 8'h1d;
            8'h42:  out <= 8'h58;
            8'h43:  out <= 8'h34;
            8'h44:  out <= 8'h36;
            8'h45:  out <= 8'hdc;
            8'h46:  out <= 8'hb4;
            8'h47:  out <= 8'h5b;
            8'h48:  out <= 8'ha4;
            8'h49:  out <= 8'h76;
            8'h4a:  out <= 8'hb7;
            8'h4b:  out <= 8'h7d;
            8'h4c:  out <= 8'h52;
            8'h4d:  out <= 8'hdd;
            8'h4e:  out <= 8'h5e;
            8'h4f:  out <= 8'h13;
            8'h50:  out <= 8'ha6;
            8'h51:  out <= 8'hb9;
            8'h52:  out <= 8'h00;
            8'h53:  out <= 8'hc1;
            8'h54:  out <= 8'h40;
            8'h55:  out <= 8'he3;
            8'h56:  out <= 8'h79;
            8'h57:  out <= 8'hb6;
            8'h58:  out <= 8'hd4;
            8'h59:  out <= 8'h8d;
            8'h5a:  out <= 8'h67;
            8'h5b:  out <= 8'h72;
            8'h5c:  out <= 8'h94;
            8'h5d:  out <= 8'h98;
            8'h5e:  out <= 8'hb0;
            8'h5f:  out <= 8'h85;
            8'h60:  out <= 8'hbb;
            8'h61:  out <= 8'hc5;
            8'h62:  out <= 8'h4f;
            8'h63:  out <= 8'hed;
            8'h64:  out <= 8'h86;
            8'h65:  out <= 8'h9a;
            8'h66:  out <= 8'h66;
            8'h67:  out <= 8'h11;
            8'h68:  out <= 8'h8a;
            8'h69:  out <= 8'he9;
            8'h6a:  out <= 8'h04;
            8'h6b:  out <= 8'hfe;
            8'h6c:  out <= 8'ha0;
            8'h6d:  out <= 8'h78;
            8'h6e:  out <= 8'h25;
            8'h6f:  out <= 8'h4b;
            8'h70:  out <= 8'ha2;
            8'h71:  out <= 8'h5d;
            8'h72:  out <= 8'h80;
            8'h73:  out <= 8'h05;
            8'h74:  out <= 8'h3f;
            8'h75:  out <= 8'h21;
            8'h76:  out <= 8'h70;
            8'h77:  out <= 8'hf1;
            8'h78:  out <= 8'h63;
            8'h79:  out <= 8'h77;
            8'h7a:  out <= 8'haf;
            8'h7b:  out <= 8'h42;
            8'h7c:  out <= 8'h20;
            8'h7d:  out <= 8'he5;
            8'h7e:  out <= 8'hfd;
            8'h7f:  out <= 8'hbf;
            8'h80:  out <= 8'h81;
            8'h81:  out <= 8'h18;
            8'h82:  out <= 8'h26;
            8'h83:  out <= 8'hc3;
            8'h84:  out <= 8'hbe;
            8'h85:  out <= 8'h35;
            8'h86:  out <= 8'h88;
            8'h87:  out <= 8'h2e;
            8'h88:  out <= 8'h93;
            8'h89:  out <= 8'h55;
            8'h8a:  out <= 8'hfc;
            8'h8b:  out <= 8'h7a;
            8'h8c:  out <= 8'hc8;
            8'h8d:  out <= 8'hba;
            8'h8e:  out <= 8'h32;
            8'h8f:  out <= 8'he6;
            8'h90:  out <= 8'hc0;
            8'h91:  out <= 8'h19;
            8'h92:  out <= 8'h9e;
            8'h93:  out <= 8'ha3;
            8'h94:  out <= 8'h44;
            8'h95:  out <= 8'h54;
            8'h96:  out <= 8'h3b;
            8'h97:  out <= 8'h0b;
            8'h98:  out <= 8'h8c;
            8'h99:  out <= 8'hc7;
            8'h9a:  out <= 8'h6b;
            8'h9b:  out <= 8'h28;
            8'h9c:  out <= 8'ha7;
            8'h9d:  out <= 8'hbc;
            8'h9e:  out <= 8'h16;
            8'h9f:  out <= 8'had;
            8'ha0:  out <= 8'hdb;
            8'ha1:  out <= 8'h64;
            8'ha2:  out <= 8'h74;
            8'ha3:  out <= 8'h14;
            8'ha4:  out <= 8'h92;
            8'ha5:  out <= 8'h0c;
            8'ha6:  out <= 8'h48;
            8'ha7:  out <= 8'hb8;
            8'ha8:  out <= 8'h9f;
            8'ha9:  out <= 8'hbd;
            8'haa:  out <= 8'h43;
            8'hab:  out <= 8'hc4;
            8'hac:  out <= 8'h39;
            8'had:  out <= 8'h31;
            8'hae:  out <= 8'hd3;
            8'haf:  out <= 8'hf2;
            8'hb0:  out <= 8'hd5;
            8'hb1:  out <= 8'h8b;
            8'hb2:  out <= 8'h6e;
            8'hb3:  out <= 8'hda;
            8'hb4:  out <= 8'h01;
            8'hb5:  out <= 8'hb1;
            8'hb6:  out <= 8'h9c;
            8'hb7:  out <= 8'h49;
            8'hb8:  out <= 8'hd8;
            8'hb9:  out <= 8'hac;
            8'hba:  out <= 8'hf3;
            8'hbb:  out <= 8'hcf;
            8'hbc:  out <= 8'hca;
            8'hbd:  out <= 8'hf4;
            8'hbe:  out <= 8'h47;
            8'hbf:  out <= 8'h10;
            8'hc0:  out <= 8'h6f;
            8'hc1:  out <= 8'hf0;
            8'hc2:  out <= 8'h4a;
            8'hc3:  out <= 8'h5c;
            8'hc4:  out <= 8'h38;
            8'hc5:  out <= 8'h57;
            8'hc6:  out <= 8'h73;
            8'hc7:  out <= 8'h97;
            8'hc8:  out <= 8'hcb;
            8'hc9:  out <= 8'ha1;
            8'hca:  out <= 8'he8;
            8'hcb:  out <= 8'h3e;
            8'hcc:  out <= 8'h96;
            8'hcd:  out <= 8'h61;
            8'hce:  out <= 8'h0d;
            8'hcf:  out <= 8'h0f;
            8'hd0:  out <= 8'he0;
            8'hd1:  out <= 8'h7c;
            8'hd2:  out <= 8'h71;
            8'hd3:  out <= 8'hcc;
            8'hd4:  out <= 8'h90;
            8'hd5:  out <= 8'h06;
            8'hd6:  out <= 8'hf7;
            8'hd7:  out <= 8'h1c;
            8'hd8:  out <= 8'hc2;
            8'hd9:  out <= 8'h6a;
            8'hda:  out <= 8'hae;
            8'hdb:  out <= 8'h69;
            8'hdc:  out <= 8'h17;
            8'hdd:  out <= 8'h99;
            8'hde:  out <= 8'h3a;
            8'hdf:  out <= 8'h27;
            8'he0:  out <= 8'hd9;
            8'he1:  out <= 8'heb;
            8'he2:  out <= 8'h2b;
            8'he3:  out <= 8'h22;
            8'he4:  out <= 8'hd2;
            8'he5:  out <= 8'ha9;
            8'he6:  out <= 8'h07;
            8'he7:  out <= 8'h33;
            8'he8:  out <= 8'h2d;
            8'he9:  out <= 8'h3c;
            8'hea:  out <= 8'h15;
            8'heb:  out <= 8'hc9;
            8'hec:  out <= 8'h87;
            8'hed:  out <= 8'haa;
            8'hee:  out <= 8'h50;
            8'hef:  out <= 8'ha5;
            8'hf0:  out <= 8'h03;
            8'hf1:  out <= 8'h59;
            8'hf2:  out <= 8'h09;
            8'hf3:  out <= 8'h1a;
            8'hf4:  out <= 8'h65;
            8'hf5:  out <= 8'hd7;
            8'hf6:  out <= 8'h84;
            8'hf7:  out <= 8'hd0;
            8'hf8:  out <= 8'h82;
            8'hf9:  out <= 8'h29;
            8'hfa:  out <= 8'h5a;
            8'hfb:  out <= 8'h1e;
            8'hfc:  out <= 8'h7b;
            8'hfd:  out <= 8'ha8;
            8'hfe:  out <= 8'h6d;
            8'hff:  out <= 8'h2c;
        endcase
endmodule

