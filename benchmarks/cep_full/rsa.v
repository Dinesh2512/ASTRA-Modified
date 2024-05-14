//======================================================================
//
// modexp.v
// --------
// Top level wrapper for the modula exponentiation core. The core
// is used to implement public key algorithms such as RSA,
// DH, ElGamal etc.
//
// The core calculates the following function:
//
//   C = M ** e mod N
//
//   M is a message with a length of n bits
//   e is the exponent with a length of m bits
//   N is the modulus  with a length of n bits
//
//   n can be 32 and up to and including 8192 bits in steps
//   of 32 bits.
//   m can be one and up to and including 8192 bits in steps
//   of 32 bits.
//
// The core has a 32-bit memory like interface, but provides
// status signals to inform the system that a given operation
// has is done. Additionally, any errors will also be asserted.
//
//
// Author: Joachim Strombergson, Peter Magnusson
// Copyright (c) 2015, NORDUnet A/S
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//======================================================================

/*
module modexp(
           input wire           clk,
           input wire           reset_n,

           input wire           cs,
           input wire           we,

           input wire  [ 7 : 0] address,
           input wire  [31 : 0] write_data,
           output wire [31 : 0] read_data
       );


//----------------------------------------------------------------
// Internal constant and parameter definitions.
//----------------------------------------------------------------

// The operand width is the internal operand width in bits.
// The address width is the size of the address space used. This
// value must be balances with OPERAND_WIDTH to allow a total
// of 8192 bits of data. OPERAND_WIDTH * (ADDRESS_WIDTH ** 2)
// is the formula. Note that the API data with is always 32 bits.
localparam OPERAND_WIDTH         = 32;
localparam ADDRESS_WIDTH         = 8;

localparam ADDR_NAME0            = 8'h00;
localparam ADDR_NAME1            = 8'h01;
localparam ADDR_VERSION          = 8'h02;

localparam ADDR_CTRL             = 8'h08;
localparam CTRL_INIT_BIT         = 0;
localparam CTRL_NEXT_BIT         = 1;

localparam ADDR_STATUS           = 8'h09;
localparam STATUS_READY_BIT      = 0;

localparam ADDR_CYCLES_HIGH      = 8'h10;
localparam ADDR_CYCLES_LOW       = 8'h11;

localparam ADDR_MODULUS_LENGTH   = 8'h20;
localparam ADDR_EXPONENT_LENGTH  = 8'h21;

localparam ADDR_MODULUS_PTR_RST  = 8'h30;
localparam ADDR_MODULUS_DATA     = 8'h31;

localparam ADDR_EXPONENT_PTR_RST = 8'h40;
localparam ADDR_EXPONENT_DATA    = 8'h41;

localparam ADDR_MESSAGE_PTR_RST  = 8'h50;
localparam ADDR_MESSAGE_DATA     = 8'h51;

localparam ADDR_RESULT_PTR_RST   = 8'h60;
localparam ADDR_RESULT_DATA      = 8'h61;

localparam DEFAULT_MODLENGTH     = 8'h80; // 2048 bits.
localparam DEFAULT_EXPLENGTH     = 8'h80;

localparam CORE_NAME0            = 32'h6d6f6465; // "mode"
localparam CORE_NAME1            = 32'h78702020; // "xp  "
localparam CORE_VERSION          = 32'h302e3532; // "0.52"


//----------------------------------------------------------------
// Registers including update variables and write enable.
//----------------------------------------------------------------
reg [07 : 0] exponent_length_reg;
reg [07 : 0] exponent_length_new;
reg          exponent_length_we;

reg [07 : 0] modulus_length_reg;
reg [07 : 0] modulus_length_new;
reg          modulus_length_we;

reg          start_reg;
reg          start_new;


//----------------------------------------------------------------
// Wires.
//----------------------------------------------------------------
reg           exponent_mem_api_rst;
reg           exponent_mem_api_cs;
reg           exponent_mem_api_wr;
wire [31 : 0] exponent_mem_api_read_data;

reg           modulus_mem_api_rst;
reg           modulus_mem_api_cs;
reg           modulus_mem_api_wr;
wire [31 : 0] modulus_mem_api_read_data;

reg           message_mem_api_rst;
reg           message_mem_api_cs;
reg           message_mem_api_wr;
wire [31 : 0] message_mem_api_read_data;

reg           result_mem_api_rst;
reg           result_mem_api_cs;
wire [31 : 0] result_mem_api_read_data;

wire          ready;
wire [63 : 0] cycles;

reg [31 : 0]  tmp_read_data;


//----------------------------------------------------------------
// Concurrent connectivity for ports etc.
//----------------------------------------------------------------
assign read_data = tmp_read_data;


//----------------------------------------------------------------
// core instantiations.
//----------------------------------------------------------------
modexp_core #(.OPW(OPERAND_WIDTH), .ADW(ADDRESS_WIDTH))
            core_inst(
                .clk(clk),
                .reset_n(reset_n),

                .start(start_reg),
                .ready(ready),

                .exponent_length(exponent_length_reg),
                .modulus_length(modulus_length_reg),

                .cycles(cycles),

                .exponent_mem_api_cs(exponent_mem_api_cs),
                .exponent_mem_api_wr(exponent_mem_api_wr),
                .exponent_mem_api_rst(exponent_mem_api_rst),
                .exponent_mem_api_write_data(write_data),
                .exponent_mem_api_read_data(exponent_mem_api_read_data),

                .modulus_mem_api_cs(modulus_mem_api_cs),
                .modulus_mem_api_wr(modulus_mem_api_wr),
                .modulus_mem_api_rst(modulus_mem_api_rst),
                .modulus_mem_api_write_data(write_data),
                .modulus_mem_api_read_data(modulus_mem_api_read_data),

                .message_mem_api_cs(message_mem_api_cs),
                .message_mem_api_wr(message_mem_api_wr),
                .message_mem_api_rst(message_mem_api_rst),
                .message_mem_api_write_data(write_data),
                .message_mem_api_read_data(message_mem_api_read_data),

                .result_mem_api_cs(result_mem_api_cs),
                .result_mem_api_rst(result_mem_api_rst),
                .result_mem_api_read_data(result_mem_api_read_data)
            );


//----------------------------------------------------------------
// reg_update
//
// Update functionality for all registers in the core.
// All registers are positive edge triggered with asynchronous
// active low reset. All registers have write enable.
//----------------------------------------------------------------
always @ (posedge clk or negedge reset_n)
    begin
        if (!reset_n)
            begin
                start_reg           <= 1'b0;
                exponent_length_reg <= DEFAULT_EXPLENGTH;
                modulus_length_reg  <= DEFAULT_MODLENGTH;
            end
        else
            begin
                start_reg <= start_new;

                if (exponent_length_we)
                    begin
                        exponent_length_reg <= write_data[7 : 0];
                    end

                if (modulus_length_we)
                    begin
                        modulus_length_reg <= write_data[7 : 0];
                    end
            end
    end // reg_update


//----------------------------------------------------------------
// api
//
// The interface command decoding logic.
//----------------------------------------------------------------
always @*
    begin : api
        modulus_length_we    = 1'b0;
        exponent_length_we   = 1'b0;
        start_new            = 1'b0;

        modulus_mem_api_rst  = 1'b0;
        modulus_mem_api_cs   = 1'b0;
        modulus_mem_api_wr   = 1'b0;

        exponent_mem_api_rst = 1'b0;
        exponent_mem_api_cs  = 1'b0;
        exponent_mem_api_wr  = 1'b0;

        message_mem_api_rst  = 1'b0;
        message_mem_api_cs   = 1'b0;
        message_mem_api_wr   = 1'b0;

        result_mem_api_rst   = 1'b0;
        result_mem_api_cs    = 1'b0;

        tmp_read_data        = 32'h00000000;

        if (cs)
            begin
                if (we)
                    begin
                        case (address)
                            ADDR_CTRL:
                                begin
                                    start_new = write_data[0];
                                end

                            ADDR_MODULUS_LENGTH:
                                begin
                                    modulus_length_we = 1'b1;
                                end

                            ADDR_EXPONENT_LENGTH:
                                begin
                                    exponent_length_we = 1'b1;
                                end

                            ADDR_MODULUS_PTR_RST:
                                begin
                                    modulus_mem_api_rst = 1'b1;
                                end

                            ADDR_MODULUS_DATA:
                                begin
                                    modulus_mem_api_cs = 1'b1;
                                    modulus_mem_api_wr = 1'b1;
                                end

                            ADDR_EXPONENT_PTR_RST:
                                begin
                                    exponent_mem_api_rst = 1'b1;
                                end

                            ADDR_EXPONENT_DATA:
                                begin
                                    exponent_mem_api_cs = 1'b1;
                                    exponent_mem_api_wr = 1'b1;
                                end

                            ADDR_MESSAGE_PTR_RST:
                                begin
                                    message_mem_api_rst = 1'b1;
                                end

                            ADDR_MESSAGE_DATA:
                                begin
                                    message_mem_api_cs = 1'b1;
                                    message_mem_api_wr = 1'b1;
                                end

                            ADDR_RESULT_PTR_RST:
                                begin
                                    result_mem_api_rst = 1'b1;
                                end

                            default:
                                begin
                                end
                        endcase // case (address[7 : 0])
                    end // if (we)
                else
                    begin
                        case (address)
                            ADDR_NAME0:
                                tmp_read_data = CORE_NAME0;

                            ADDR_NAME1:
                                tmp_read_data = CORE_NAME1;

                            ADDR_VERSION:
                                tmp_read_data = CORE_VERSION;

                            ADDR_CTRL:
                                tmp_read_data = {31'h00000000, start_reg};

                            ADDR_STATUS:
                                tmp_read_data = {31'h00000000, ready};

                            ADDR_CYCLES_HIGH:
                                tmp_read_data = cycles[63 : 32];

                            ADDR_CYCLES_LOW:
                                tmp_read_data = cycles[31 : 0];

                            ADDR_MODULUS_LENGTH:
                                tmp_read_data = {24'h000000, modulus_length_reg};

                            ADDR_EXPONENT_LENGTH:
                                tmp_read_data = {24'h000000, exponent_length_reg};

                            ADDR_MODULUS_DATA:
                                begin
                                    modulus_mem_api_cs = 1'b1;
                                    tmp_read_data      = modulus_mem_api_read_data;
                                end

                            ADDR_EXPONENT_DATA:
                                begin
                                    exponent_mem_api_cs = 1'b1;
                                    tmp_read_data       = exponent_mem_api_read_data;
                                end

                            ADDR_MESSAGE_DATA:
                                begin
                                    message_mem_api_cs = 1'b1;
                                    tmp_read_data      = message_mem_api_read_data;
                                end

                            ADDR_RESULT_DATA:
                                begin
                                    result_mem_api_cs = 1'b1;
                                    tmp_read_data     = result_mem_api_read_data;
                                end

                            default:
                                begin
                                end
                        endcase // case (address)
                    end // else: !if(we)
            end // if (cs)
    end // block: api

endmodule // modexp

          //======================================================================
          // EOF modexp.v
          //======================================================================

*/

//======================================================================
//
// modexp_core.v
// -------------
// Modular exponentiation core for implementing public key algorithms
// such as RSA, DH, ElGamal etc.
//
// The core calculates the following function:
//
//   C = M ** e mod N
//
//   M is a message with a length of n bits
//   e is the exponent with a length of m bits
//   N is the modulus  with a length of n bits
//
//   n can be 32 and up to and including 8192 bits in steps
//   of 32 bits.
//   m can be one and up to and including 8192 bits in steps
//   of 32 bits.
//
// The core has access ports for the exponent, modulus, message and
// result memories.
//
//
// Author: Joachim Strombergson, Peter Magnusson
// Copyright (c) 2015, NORDUnet A/S
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//======================================================================

module modexp_core #(parameter OPW = 32, parameter ADW = 8)
       (
           input wire 		clk,
           input wire 		reset_n,

           input wire 		start,
           output wire 		ready,

//           input wire [07 : 0]  exponent_length,
// tony duong 05/27/20: change from unit of 32-bits to actual bits to speed up
           input wire [12 : 0] 	exponent_length, 
           input wire [07 : 0] 	modulus_length,

           output wire [63 : 0] cycles,

           input wire 		exponent_mem_api_cs,
           input wire 		exponent_mem_api_wr,
           input wire 		exponent_mem_api_rst,
           input wire [31 : 0] 	exponent_mem_api_write_data,
           output wire [31 : 0] exponent_mem_api_read_data,

           input wire 		modulus_mem_api_cs,
           input wire 		modulus_mem_api_wr,
           input wire 		modulus_mem_api_rst,
           input wire [31 : 0] 	modulus_mem_api_write_data,
           output wire [31 : 0] modulus_mem_api_read_data,

           input wire 		message_mem_api_cs,
           input wire 		message_mem_api_wr,
           input wire 		message_mem_api_rst,
           input wire [31 : 0] 	message_mem_api_write_data,
           output wire [31 : 0] message_mem_api_read_data,

           input wire 		result_mem_api_cs,
           input wire 		result_mem_api_rst,
           output wire [31 : 0] result_mem_api_read_data
       );


//----------------------------------------------------------------
// Internal constant and parameter definitions.
//----------------------------------------------------------------
localparam MONTPROD_SELECT_ONE_NR = 3'h0;
localparam MONTPROD_SELECT_X_NR   = 3'h1;
localparam MONTPROD_SELECT_Z_P    = 3'h2;
localparam MONTPROD_SELECT_P_P    = 3'h3;
localparam MONTPROD_SELECT_Z_ONE  = 3'h4;

localparam MONTPROD_DEST_Z        = 2'b00;
localparam MONTPROD_DEST_P        = 2'b01;
localparam MONTPROD_DEST_NOWHERE  = 2'b10;

localparam CTRL_IDLE              = 4'h0;
localparam CTRL_RESIDUE           = 4'h1;
localparam CTRL_CALCULATE_Z0      = 4'h2;
localparam CTRL_CALCULATE_P0      = 4'h3;
localparam CTRL_ITERATE           = 4'h4;
localparam CTRL_ITERATE_Z_P       = 4'h5;
localparam CTRL_ITERATE_P_P       = 4'h6;
localparam CTRL_ITERATE_END       = 4'h7;
localparam CTRL_CALCULATE_ZN      = 4'h8;
localparam CTRL_DONE              = 4'h9;

//for rsa, c=M^65537 etc, there is no need to slow down to hide the exponent
localparam EXPONATION_MODE_SECRET_SECURE = 1'b0;
localparam EXPONATION_MODE_PUBLIC_FAST   = 1'b1;


//----------------------------------------------------------------
// Registers including update variables and write enable.
//----------------------------------------------------------------
reg          ready_reg;
reg          ready_new;
reg          ready_we;

reg [2 : 0]  montprod_select_reg;
reg [2 : 0]  montprod_select_new;
reg          montprod_select_we;
reg [1 : 0]  montprod_dest_reg;
reg [1 : 0]  montprod_dest_new;
reg          montprod_dest_we;

reg [3 : 0]  modexp_ctrl_reg;
reg [3 : 0]  modexp_ctrl_new;
reg          modexp_ctrl_we;

reg [31 : 0] one_reg;
reg [31 : 0] one_new;
reg [31 : 0] b_one_reg;
reg [31 : 0] b_one_new;

reg [12 : 0] loop_counter_reg;
reg [12 : 0] loop_counter_new;
reg          loop_counter_we;

reg [07 : 0] E_word_index;
reg [04 : 0] E_bit_index;
reg          last_iteration;

reg          ei_reg;
reg          ei_new;
reg          ei_we;

reg          exponation_mode_reg;
reg          exponation_mode_new;
reg          exponation_mode_we;

reg [31 : 0] cycle_ctr_low_reg;
reg [31 : 0] cycle_ctr_low_new;
reg          cycle_ctr_low_we;
reg [31 : 0] cycle_ctr_high_reg;
reg [31 : 0] cycle_ctr_high_new;
reg          cycle_ctr_high_we;
reg          cycle_ctr_state_reg;
reg          cycle_ctr_state_new;
reg          cycle_ctr_state_we;
reg          cycle_ctr_start;
reg          cycle_ctr_stop;


//----------------------------------------------------------------
// Wires.
//----------------------------------------------------------------
reg [07 : 0]  modulus_mem_int_rd_addr;
wire [31 : 0] modulus_mem_int_rd_data;

reg [07 : 0]  message_mem_int_rd_addr;
wire [31 : 0] message_mem_int_rd_data;

reg [07 : 0]  exponent_mem_int_rd_addr;
wire [31 : 0] exponent_mem_int_rd_data;

reg  [07 : 0] result_mem_int_rd_addr;
wire [31 : 0] result_mem_int_rd_data;
reg  [07 : 0] result_mem_int_wr_addr;
reg  [31 : 0] result_mem_int_wr_data;
reg           result_mem_int_we;

reg  [07 : 0] p_mem_rd0_addr;
wire [31 : 0] p_mem_rd0_data;
reg  [07 : 0] p_mem_rd1_addr;
wire [31 : 0] p_mem_rd1_data;
reg  [07 : 0] p_mem_wr_addr;
reg  [31 : 0] p_mem_wr_data;
reg           p_mem_we;

reg [31 : 0]  tmp_read_data;

reg           montprod_calc;
wire          montprod_ready;
reg [07 : 0]  montprod_length;

wire [07 : 0] montprod_opa_addr;
reg [31 : 0]  montprod_opa_data;

wire [07 : 0] montprod_opb_addr;
reg [31 : 0]  montprod_opb_data;

wire [07 : 0] montprod_opm_addr;
reg [31 : 0]  montprod_opm_data;

wire [07 : 0] montprod_result_addr;
wire [31 : 0] montprod_result_data;
wire          montprod_result_we;

reg           residue_calculate;
wire          residue_ready;
reg [14 : 0]  residue_nn;
reg  [07 : 0] residue_length;
wire [07 : 0] residue_opa_rd_addr;
wire [31 : 0] residue_opa_rd_data;
wire [07 : 0] residue_opa_wr_addr;
wire [31 : 0] residue_opa_wr_data;
wire          residue_opa_wr_we;
wire [07 : 0] residue_opm_addr;
reg [31 : 0]  residue_opm_data;

reg  [07 : 0] residue_mem_montprod_read_addr;
wire [31 : 0] residue_mem_montprod_read_data;

reg           residue_valid_reg;
reg           residue_valid_new;
reg           residue_valid_int_validated;

wire [7 : 0]  modulus_length_m1;
   // tony duong
//wire [7 : 0]  exponent_length_m1;
wire [12 : 0]  exponent_length_m1;   


//----------------------------------------------------------------
// Concurrent connectivity for ports etc.
//----------------------------------------------------------------
assign ready  = ready_reg;
assign cycles = {cycle_ctr_high_reg, cycle_ctr_low_reg};

assign modulus_length_m1  = modulus_length - 8'h1;
assign exponent_length_m1 = exponent_length - 8'h1;


//----------------------------------------------------------------
// core instantiations.
//----------------------------------------------------------------
montprod #(.OPW(OPW), .ADW(ADW))
         montprod_inst(
             .clk(clk),
             .reset_n(reset_n),

             .calculate(montprod_calc),
             .ready(montprod_ready),

             .length(montprod_length),

             .opa_addr(montprod_opa_addr),
             .opa_data(montprod_opa_data),

             .opb_addr(montprod_opb_addr),
             .opb_data(montprod_opb_data),

             .opm_addr(montprod_opm_addr),
             .opm_data(montprod_opm_data),

             .result_addr(montprod_result_addr),
             .result_data(montprod_result_data),
             .result_we(montprod_result_we)
         );


residue #(.OPW(OPW), .ADW(ADW))
        residue_inst(
            .clk(clk),
            .reset_n(reset_n),
            .calculate(residue_calculate),
            .ready(residue_ready),
            .nn(residue_nn),
            .length(residue_length),
            .opa_rd_addr(residue_opa_rd_addr),
            .opa_rd_data(residue_opa_rd_data),
            .opa_wr_addr(residue_opa_wr_addr),
            .opa_wr_data(residue_opa_wr_data),
            .opa_wr_we(residue_opa_wr_we),
            .opm_addr(residue_opm_addr),
            .opm_data(residue_opm_data)
        );


blockmem2r1w #(.OPW(OPW), .ADW(ADW))
             residue_mem(
                 .clk(clk),
                 .read_addr0(residue_opa_rd_addr),
                 .read_data0(residue_opa_rd_data),
                 .read_addr1(residue_mem_montprod_read_addr),
                 .read_data1(residue_mem_montprod_read_data),
                 .wr(residue_opa_wr_we),
                 .write_addr(residue_opa_wr_addr),
                 .write_data(residue_opa_wr_data)
             );


blockmem2r1w #(.OPW(OPW), .ADW(ADW))
             p_mem(
                 .clk(clk),
                 .read_addr0(p_mem_rd0_addr),
                 .read_data0(p_mem_rd0_data),
                 .read_addr1(p_mem_rd1_addr),
                 .read_data1(p_mem_rd1_data),
                 .wr(p_mem_we),
                 .write_addr(p_mem_wr_addr),
                 .write_data(p_mem_wr_data)
             );


blockmem2r1wptr #(.OPW(OPW), .ADW(ADW))
                exponent_mem(
                    .clk(clk),
                    .reset_n(reset_n),
                    .read_addr0(exponent_mem_int_rd_addr),
                    .read_data0(exponent_mem_int_rd_data),
                    .read_data1(exponent_mem_api_read_data),
                    .rst(exponent_mem_api_rst),
                    .cs(exponent_mem_api_cs),
                    .wr(exponent_mem_api_wr),
                    .write_data(exponent_mem_api_write_data)
                );


blockmem2r1wptr #(.OPW(OPW), .ADW(ADW))
                modulus_mem(
                    .clk(clk),
                    .reset_n(reset_n),
                    .read_addr0(modulus_mem_int_rd_addr),
                    .read_data0(modulus_mem_int_rd_data),
                    .read_data1(modulus_mem_api_read_data),
                    .rst(modulus_mem_api_rst),
                    .cs(modulus_mem_api_cs),
                    .wr(modulus_mem_api_wr),
                    .write_data(modulus_mem_api_write_data)
                );


blockmem2r1wptr #(.OPW(OPW), .ADW(ADW))
                message_mem(
                    .clk(clk),
                    .reset_n(reset_n),
                    .read_addr0(message_mem_int_rd_addr),
                    .read_data0(message_mem_int_rd_data),
                    .read_data1(message_mem_api_read_data),
                    .rst(message_mem_api_rst),
                    .cs(message_mem_api_cs),
                    .wr(message_mem_api_wr),
                    .write_data(message_mem_api_write_data)
                );


blockmem2rptr1w #(.OPW(OPW), .ADW(ADW))
                result_mem(
                    .clk(clk),
                    .reset_n(reset_n),
                    .read_addr0(result_mem_int_rd_addr[7 : 0]),
                    .read_data0(result_mem_int_rd_data),
                    .read_data1(result_mem_api_read_data),
                    .rst(result_mem_api_rst),
                    .cs(result_mem_api_cs),
                    .wr(result_mem_int_we),
                    .write_addr(result_mem_int_wr_addr),
                    .write_data(result_mem_int_wr_data)
                );


//----------------------------------------------------------------
// reg_update
//
// Update functionality for all registers in the core.
// All registers are positive edge triggered with asynchronous
// active low reset. All registers have write enable.
//----------------------------------------------------------------
always @ (posedge clk or negedge reset_n)
    begin
        if (!reset_n)
            begin
                ready_reg           <= 1'b1;
                montprod_select_reg <= MONTPROD_SELECT_ONE_NR;
                montprod_dest_reg   <= MONTPROD_DEST_NOWHERE;
                modexp_ctrl_reg     <= CTRL_IDLE;
                one_reg             <= 32'h0;
                b_one_reg           <= 32'h0;
                loop_counter_reg    <= 13'b0;
                ei_reg              <= 1'b0;
                residue_valid_reg   <= 1'b0;
                exponation_mode_reg <= EXPONATION_MODE_SECRET_SECURE;
                cycle_ctr_low_reg   <= 32'h00000000;
                cycle_ctr_high_reg  <= 32'h00000000;
                cycle_ctr_state_reg <= 1'b0;
            end
        else
            begin
                one_reg             <= one_new;
                b_one_reg           <= b_one_new;
                residue_valid_reg   <= residue_valid_new;

                if (ready_we)
                    ready_reg <= ready_new;

                if (montprod_select_we)
                    montprod_select_reg <= montprod_select_new;

                if (montprod_dest_we)
                    montprod_dest_reg <= montprod_dest_new;

                if (loop_counter_we)
                    loop_counter_reg <= loop_counter_new;

                if (ei_we)
                    ei_reg <= ei_new;

                if (exponation_mode_we)
                    exponation_mode_reg <= exponation_mode_new;

                if (cycle_ctr_low_we)
                    cycle_ctr_low_reg <= cycle_ctr_low_new;

                if (cycle_ctr_high_we)
                    cycle_ctr_high_reg <= cycle_ctr_high_new;

                if (cycle_ctr_state_we)
                    cycle_ctr_state_reg <= cycle_ctr_state_new;

                if (modexp_ctrl_we)
                    modexp_ctrl_reg <= modexp_ctrl_new;
            end
    end // reg_update


//----------------------------------------------------------------
// cycle_ctr
//
// Implementation of the cycle counter
//----------------------------------------------------------------
always @*
    begin : cycle_ctr
        cycle_ctr_low_new   = 32'h00000000;
        cycle_ctr_low_we    = 1'b0;
        cycle_ctr_high_new  = 32'h00000000;
        cycle_ctr_high_we   = 1'b0;
        cycle_ctr_state_new = 1'b0;
        cycle_ctr_state_we  = 1'b0;

        if (cycle_ctr_start)
            begin
                cycle_ctr_low_new   = 32'h00000000;
                cycle_ctr_low_we    = 1'b1;
                cycle_ctr_high_new  = 32'h00000000;
                cycle_ctr_high_we   = 1'b1;
                cycle_ctr_state_new = 1'b1;
                cycle_ctr_state_we  = 1'b1;
            end

        if (cycle_ctr_stop)
            begin
                cycle_ctr_state_new = 1'b0;
                cycle_ctr_state_we  = 1'b1;
            end

        if (cycle_ctr_state_reg)
            begin
                cycle_ctr_low_new = cycle_ctr_low_reg + 1'b1;
                cycle_ctr_low_we  = 1'b1;

                if (cycle_ctr_low_new == 32'h00000000)
                    begin
                        cycle_ctr_high_new  = cycle_ctr_high_reg + 1'b1;
                        cycle_ctr_high_we   = 1'b1;
                    end
            end
    end // cycle_ctr


//----------------------------------------------------------------
// one
//
// generates the big integer one ( 00... 01 )
//----------------------------------------------------------------
always @*
    begin : one_process
        one_new   = 32'h00000000;
        b_one_new = 32'h00000000;

        if (montprod_opa_addr == modulus_length_m1)
            one_new = 32'h00000001;

        if (montprod_opb_addr == modulus_length_m1)
            b_one_new = 32'h00000001;
    end


//----------------------------------------------------------------
// Read mux for modulus. Needed since it is being
// addressed by two sources.
//----------------------------------------------------------------
always @*
    begin : modulus_mem_reader_process
        if (modexp_ctrl_reg == CTRL_RESIDUE)
            modulus_mem_int_rd_addr = residue_opm_addr;
        else
            modulus_mem_int_rd_addr = montprod_opm_addr;
    end


//----------------------------------------------------------------
// Feeds residue calculator.
//----------------------------------------------------------------
always @*
    begin : residue_process
        //N*2, N=length*32, *32 = shl5, *64 = shl6
        residue_nn = { 1'b0, modulus_length, 6'h0 };
        residue_length = modulus_length;
        residue_opm_data = modulus_mem_int_rd_data;
    end


//----------------------------------------------------------------
// Detects if modulus has been updated and we need to
// recalculate the residue
// and we need residue is valid or not.
//----------------------------------------------------------------
always @*
    begin : residue_valid_process
        residue_valid_new = residue_valid_reg;

        if (modulus_mem_api_cs & modulus_mem_api_wr)
            residue_valid_new = 1'b0;
        else if ( residue_valid_int_validated == 1'b1)
            residue_valid_new = 1'b1;
    end


//----------------------------------------------------------------
// montprod_op_select
//
// Select operands used during montprod calculations depending
// on what operation we want to do.
//----------------------------------------------------------------
always @*
    begin : montprod_op_select

        montprod_length          = modulus_length;

        result_mem_int_rd_addr   = montprod_opa_addr;
        message_mem_int_rd_addr  = montprod_opa_addr;
        p_mem_rd0_addr           = montprod_opa_addr;

        residue_mem_montprod_read_addr = montprod_opb_addr;
        p_mem_rd1_addr                 = montprod_opb_addr;

        montprod_opm_data = modulus_mem_int_rd_data;
        case (montprod_select_reg)
            MONTPROD_SELECT_ONE_NR:
                begin
                    montprod_opa_data  = one_reg;
                    montprod_opb_data  = residue_mem_montprod_read_data;
                end

            MONTPROD_SELECT_X_NR:
                begin
                    montprod_opa_data  = message_mem_int_rd_data;
                    montprod_opb_data  = residue_mem_montprod_read_data;
                end

            MONTPROD_SELECT_Z_P:
                begin
                    montprod_opa_data  = result_mem_int_rd_data;
                    montprod_opb_data  = p_mem_rd1_data;
                end

            MONTPROD_SELECT_P_P:
                begin
                    montprod_opa_data  = p_mem_rd0_data;
                    montprod_opb_data  = p_mem_rd1_data;
                end

            MONTPROD_SELECT_Z_ONE:
                begin
                    montprod_opa_data  = result_mem_int_rd_data;
                    montprod_opb_data  = b_one_reg;
                end

            default:
                begin
                    montprod_opa_data  = 32'h00000000;
                    montprod_opb_data  = 32'h00000000;
                end
        endcase // case (montprod_selcect_reg)
    end


//----------------------------------------------------------------
// memory write mux
//
// Direct memory write signals to correct memory.
//----------------------------------------------------------------
always @*
    begin : memory_write_process
        result_mem_int_wr_addr = montprod_result_addr;
        result_mem_int_wr_data = montprod_result_data;
        result_mem_int_we      = 1'b0;

        p_mem_wr_addr = montprod_result_addr;
        p_mem_wr_data = montprod_result_data;
        p_mem_we      = 1'b0;

        case (montprod_dest_reg)
            MONTPROD_DEST_Z:
                result_mem_int_we = montprod_result_we;
            MONTPROD_DEST_P:
                p_mem_we = montprod_result_we;
            default:
                begin
                end
        endcase

        // inhibit Z=Z*P when ei = 0
        if (modexp_ctrl_reg == CTRL_ITERATE_Z_P)
            result_mem_int_we = result_mem_int_we & ei_reg;
    end


//----------------------------------------------------------------
// loop_counter
//
// Calculate the loop counter and related variables.
//----------------------------------------------------------------
always @*
    begin : loop_counters_process
        loop_counter_new = 13'b0;
        loop_counter_we  = 1'b0;

       // tony duong
//        if (loop_counter_reg == {exponent_length_m1, 5'b11111})
       if (loop_counter_reg == exponent_length_m1)
            last_iteration = 1'b1;
        else
            last_iteration = 1'b0;

        case (modexp_ctrl_reg)
            CTRL_CALCULATE_P0:
                begin
                    loop_counter_new = 13'b0;
                    loop_counter_we  = 1'b1;
                end

            CTRL_ITERATE_END:
                begin
                    loop_counter_new = loop_counter_reg + 1'b1;
                    loop_counter_we  = 1'b1;
                end

            default:
                begin
                end
        endcase
    end


//----------------------------------------------------------------
// exponent
//
// Reads the exponent.
//----------------------------------------------------------------
always @*
    begin : exponent_process
        // Accessing new instead of reg - pick up update at
        // CTRL_ITERATE_NEW to remove a pipeline stall.
// tony duong       
//        E_word_index  = exponent_length_m1 - loop_counter_new[ 12 : 5 ];
        E_word_index  = exponent_length_m1[12:5] - loop_counter_new[ 12 : 5 ];	   

        E_bit_index   = loop_counter_reg[ 04 : 0 ];

        exponent_mem_int_rd_addr = E_word_index;

        ei_new = exponent_mem_int_rd_data[ E_bit_index ];

        if (modexp_ctrl_reg == CTRL_ITERATE)
            ei_we = 1'b1;
        else
            ei_we = 1'b0;
    end


//----------------------------------------------------------------
// modexp_ctrl
//
// Control FSM logic needed to perform the modexp operation.
//----------------------------------------------------------------
always @*
    begin
        ready_new           = 1'b0;
        ready_we            = 1'b0;
        montprod_select_new = MONTPROD_SELECT_ONE_NR;
        montprod_select_we  = 0;
        montprod_dest_new   = MONTPROD_DEST_NOWHERE;
        montprod_dest_we    = 0;
        montprod_calc       = 0;
        modexp_ctrl_new     = CTRL_IDLE;
        modexp_ctrl_we      = 1'b0;
        cycle_ctr_start     = 1'b0;
        cycle_ctr_stop      = 1'b0;

        residue_calculate = 1'b0;

        residue_valid_int_validated = 1'b0;

        case (modexp_ctrl_reg)
            CTRL_IDLE:
                begin
                    if (start)
                        begin
                            ready_new       = 1'b0;
                            ready_we        = 1'b1;
                            cycle_ctr_start = 1'b1;

                            if (residue_valid_reg)
                                begin
                                    //residue has alrady been calculated, start with MONTPROD( 1, Nr, MODULUS )
                                    montprod_select_new = MONTPROD_SELECT_ONE_NR;
                                    montprod_select_we  = 1;
                                    montprod_dest_new   = MONTPROD_DEST_Z;
                                    montprod_dest_we    = 1;
                                    montprod_calc       = 1;
                                    modexp_ctrl_new     = CTRL_CALCULATE_Z0;
                                    modexp_ctrl_we      = 1;
                                end
                            else
                                begin
                                    //modulus has been written and residue (Nr) must be calculated
                                    modexp_ctrl_new = CTRL_RESIDUE;
                                    modexp_ctrl_we  = 1;
                                    residue_calculate = 1'b1;
                                end
                        end
                end

            CTRL_RESIDUE:
                begin
                    if (residue_ready)
                        begin
                            montprod_select_new = MONTPROD_SELECT_ONE_NR;
                            montprod_select_we  = 1;
                            montprod_dest_new   = MONTPROD_DEST_Z;
                            montprod_dest_we    = 1;
                            montprod_calc       = 1;
                            modexp_ctrl_new     = CTRL_CALCULATE_Z0;
                            modexp_ctrl_we      = 1;
                            residue_valid_int_validated = 1'b1; //update registers telling residue is valid
                        end
                end

            CTRL_CALCULATE_Z0:
                begin
                    if (montprod_ready)
                        begin
                            montprod_select_new = MONTPROD_SELECT_X_NR;
                            montprod_select_we  = 1;
                            montprod_dest_new   = MONTPROD_DEST_P;
                            montprod_dest_we    = 1;
                            montprod_calc       = 1;
                            modexp_ctrl_new = CTRL_CALCULATE_P0;
                            modexp_ctrl_we  = 1;
                        end
                end

            CTRL_CALCULATE_P0:
                begin
                    if (montprod_ready == 1'b1)
                        begin
                            modexp_ctrl_new = CTRL_ITERATE;
                            modexp_ctrl_we  = 1;
                        end
                end

            CTRL_ITERATE:
                begin
                    montprod_select_new = MONTPROD_SELECT_Z_P;
                    montprod_select_we  = 1;
                    montprod_dest_new   = MONTPROD_DEST_Z;
                    montprod_dest_we    = 1;
                    montprod_calc       = 1;
                    modexp_ctrl_new     = CTRL_ITERATE_Z_P;
                    modexp_ctrl_we      = 1;

                    if (ei_new == 1'b0 && exponation_mode_reg == EXPONATION_MODE_PUBLIC_FAST)
                        begin
                            //Skip the fake montgomery calculation, exponation_mode_reg optimizing for speed not blinding.
                            montprod_select_new = MONTPROD_SELECT_P_P;
                            montprod_dest_new   = MONTPROD_DEST_P;
                            modexp_ctrl_new     = CTRL_ITERATE_P_P;
                        end
                end

            CTRL_ITERATE_Z_P:
                if (montprod_ready)
                    begin
                        montprod_select_new = MONTPROD_SELECT_P_P;
                        montprod_select_we  = 1;
                        montprod_dest_new   = MONTPROD_DEST_P;
                        montprod_dest_we    = 1;
                        montprod_calc       = 1;
                        modexp_ctrl_new     = CTRL_ITERATE_P_P;
                        modexp_ctrl_we      = 1;
                    end

            CTRL_ITERATE_P_P:
                if (montprod_ready == 1'b1)
                    begin
                        modexp_ctrl_new = CTRL_ITERATE_END;
                        modexp_ctrl_we  = 1;
                    end

            CTRL_ITERATE_END:
                begin
                    if (!last_iteration)
                        begin
                            modexp_ctrl_new = CTRL_ITERATE;
                            modexp_ctrl_we  = 1;
                        end
                    else
                        begin
                            montprod_select_new = MONTPROD_SELECT_Z_ONE;
                            montprod_select_we  = 1;
                            montprod_dest_new   = MONTPROD_DEST_Z;
                            montprod_dest_we    = 1;
                            montprod_calc       = 1;
                            modexp_ctrl_new     = CTRL_CALCULATE_ZN;
                            modexp_ctrl_we      = 1;
                        end
                end

            CTRL_CALCULATE_ZN:
                begin
                    if (montprod_ready)
                        begin
                            modexp_ctrl_new = CTRL_DONE;
                            modexp_ctrl_we  = 1;
                        end
                end

            CTRL_DONE:
                begin
                    cycle_ctr_stop  = 1'b1;
                    ready_new       = 1'b1;
                    ready_we        = 1'b1;
                    modexp_ctrl_new = CTRL_IDLE;
                    modexp_ctrl_we  = 1;
                end

            default:
                begin
                end

        endcase // case (modexp_ctrl_reg)
    end

endmodule // modexp_core

          //======================================================================
          // EOF modexp_core.v
          //======================================================================


//======================================================================
//
// adder.v
// -------
// Adder with separate carry in and carry out. Used in the montprod
// amd residue modules of the modexp core.
//
//
// Author: Peter Magnusson, Joachim Strömbergson
// Copyright (c) 2015, NORDUnet A/S All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//======================================================================

module adder #(parameter OPW = 32)
       (
           input      [(OPW - 1) : 0]  a,
           input      [(OPW - 1) : 0]  b,
           input                       carry_in,

           output wire [(OPW - 1) : 0] sum,
           output wire                 carry_out
       );

reg [(OPW) : 0] adder_result;

assign sum = adder_result[(OPW - 1) : 0];
assign carry_out = adder_result[(OPW)];

always @*
    begin
        adder_result = {1'b0, a} + {1'b0, b} + {{OPW{1'b0}}, carry_in};
    end

endmodule // adder

          //======================================================================
          // EOF adder.v
          //======================================================================

//======================================================================
//
// blockmem1rw1.v
// --------------
// Synchronous block memory with one read and one write port.
// The data size is the same for both read and write operations.
//
// The memory is used in the modexp core.
//
// paremeter OPW is operand word width in bits.
// parameter ADW is address width in bits.
//
//
// Author: Joachim Strombergson
// Copyright (c) 2015, NORDUnet A/S All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//======================================================================

module blockmem1r1w #(parameter OPW = 32, parameter ADW = 8)
       (
           input wire                  clk,

           input wire  [(ADW - 1) : 0] read_addr,
           output wire [(OPW - 1) : 0] read_data,

           input wire                  wr,
           input wire  [(ADW - 1) : 0] write_addr,
           input wire  [(OPW - 1) : 0] write_data
       );

reg [(OPW - 1) : 0] mem [0 : ((2**ADW) - 1)];
reg [(OPW - 1) : 0] tmp_read_data;

assign read_data = tmp_read_data;

always @ (posedge clk)
    begin : reg_mem
        if (wr)
            mem[write_addr] <= write_data;

        tmp_read_data <= mem[read_addr];
    end

endmodule // blockmem1r1w

          //======================================================================
          // EOF blockmem1r1w.v
          //======================================================================


//======================================================================
//
// blockmem2r1w.v
// --------------
// Synchronous block memory with two read ports and one write port.
// The data size is the same for both read and write operations.
//
// The memory is used in the modexp core.
//
//
// Author: Joachim Strombergson
// Copyright (c) 2015, NORDUnet A/S All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//======================================================================

module blockmem2r1w #(parameter OPW = 32, parameter ADW = 8)
       (
           input wire                  clk,

           input wire  [(ADW - 1) : 0] read_addr0,
           output wire [(OPW - 1) : 0] read_data0,

           input wire  [(ADW - 1) : 0] read_addr1,
           output wire [(OPW - 1) : 0] read_data1,

           input wire                  wr,
           input wire  [(ADW - 1) : 0] write_addr,
           input wire  [(OPW - 1) : 0] write_data
       );

reg [(OPW - 1) : 0] mem [0 : ((2**ADW) - 1)];
reg [(OPW - 1) : 0] tmp_read_data0;
reg [(OPW - 1) : 0] tmp_read_data1;

assign read_data0 = tmp_read_data0;
assign read_data1 = tmp_read_data1;

always @ (posedge clk)
    begin : reg_mem
        if (wr)
            mem[write_addr] <= write_data;

        tmp_read_data0 <= mem[read_addr0];
        tmp_read_data1 <= mem[read_addr1];
    end

endmodule // blockmem2r1w

          //======================================================================
          // EOF blockmem2r1w.v
          //======================================================================


//======================================================================
//
// blockmem2r1wptr.v
// -----------------
// Synchronous block memory with two read ports and one write port.
// For port 1 the address is implicit and instead given by the
// internal pointer. The pointer is automatically increased
// when the cs signal is set. The pointer is reset to zero when
// the rst signal is asserted.
//
//
// NOTE: This memory needs to be rebuilt if interface 0 is changed
// to use bigger operand widths and fewer words than interface 1.
// This adaption is NOT automatic.
//
//
// The memory is used in the modexp core.
//
//
// Author: Joachim Strombergson
// Copyright (c) 2015, NORDUnet A/S All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//======================================================================

module blockmem2r1wptr #(parameter OPW = 32, parameter ADW = 8)
       (
           input wire                  clk,
           input wire                  reset_n,

           input wire  [(ADW - 1) : 0] read_addr0,
           output wire [(OPW - 1) : 0] read_data0,

           output wire [31 : 0]        read_data1,
           input wire                  rst,
           input wire                  cs,
           input wire                  wr,
           input wire  [31 : 0]        write_data
       );


//----------------------------------------------------------------
// Memories and regs including update variables and write enable.
//----------------------------------------------------------------
reg [(OPW - 1) : 0] mem [0 : ((2**ADW) - 1)];
reg [(OPW - 1) : 0] tmp_read_data0;
reg [31 : 0]        tmp_read_data1;

reg [7 : 0]         ptr_reg;
reg [7 : 0]         ptr_new;
reg                 ptr_we;


//----------------------------------------------------------------
// Concurrent connectivity for ports etc.
//----------------------------------------------------------------
assign read_data0 = tmp_read_data0;
assign read_data1 = tmp_read_data1;


//----------------------------------------------------------------
// mem_update
//
// Clocked update of memory This should cause
// the memory to be implemented as a block memory.
//----------------------------------------------------------------
always @ (posedge clk)
    begin : mem_update
        if (wr)
            mem[ptr_reg] <= write_data;

        tmp_read_data0 <= mem[read_addr0];
        tmp_read_data1 <= mem[ptr_reg];
    end


//----------------------------------------------------------------
// ptr_update
//----------------------------------------------------------------
always @ (posedge clk or negedge reset_n)
    begin : ptr_update
        if (!reset_n)
            ptr_reg <= 8'h00;

        else
            if (ptr_we)
                ptr_reg <= ptr_new;
    end


//----------------------------------------------------------------
// ptr_logic
//----------------------------------------------------------------
always @*
    begin : ptr_logic
        ptr_new = 8'h00;
        ptr_we  = 1'b0;

        if (rst)
            begin
                ptr_new = 8'h00;
                ptr_we  = 1'b1;
            end

        if (cs)
            begin
                ptr_new = ptr_reg + 1'b1;
                ptr_we  = 1'b1;
            end
    end

endmodule // blockmem2r1wptr

          //======================================================================
          // EOF blockmem2r1wptr.v
          //======================================================================


//======================================================================
//
// blockmem2r1wptr.v
// -----------------
// Synchronous block memory with two read ports and one write port.
// For port 1 the address is implicit and instead given by the
// internal pointer. But write address is explicitly given.
//
// The memory is used in the modexp core.
//
//
// NOTE: This memory needs to be rebuilt if interface 0 is changed
// to use bigger operand widths and fewer words than interface 1.
// This adaption is NOT automatic.
//
//
// Author: Joachim Strombergson
// Copyright (c) 2015, NORDUnet A/S All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//======================================================================

module blockmem2rptr1w #(parameter OPW = 32, parameter ADW = 8)
       (
           input wire                  clk,
           input wire                  reset_n,

           input wire  [(ADW - 1) : 0] read_addr0,
           output wire [(OPW - 1) : 0] read_data0,

           output wire [31 : 0]        read_data1,
           input wire                  rst,
           input wire                  cs,
           input wire                  wr,
           input wire  [07 : 0]        write_addr,
           input wire  [31 : 0]        write_data
       );


//----------------------------------------------------------------
// Memories and regs including update variables and write enable.
//----------------------------------------------------------------
reg [(OPW - 1) : 0] mem [0 : ((2**ADW) - 1)];
reg [(OPW - 1) : 0] tmp_read_data0;
reg [31 : 0]        tmp_read_data1;

reg [7 : 0]         ptr_reg;
reg [7 : 0]         ptr_new;
reg                 ptr_we;


//----------------------------------------------------------------
// Concurrent connectivity for ports etc.
//----------------------------------------------------------------
assign read_data0 = tmp_read_data0;
assign read_data1 = tmp_read_data1;


//----------------------------------------------------------------
// mem_update
//
// Clocked update of memory This should cause
// the memory to be implemented as a block memory.
//----------------------------------------------------------------
always @ (posedge clk)
    begin : mem_update
        if (wr)
            mem[write_addr] <= write_data;

        tmp_read_data0 <= mem[read_addr0];
        tmp_read_data1 <= mem[ptr_reg];
    end


//----------------------------------------------------------------
// reg_update
//----------------------------------------------------------------
always @ (posedge clk or negedge reset_n)
    begin : reg_mem_update
        if (!reset_n)
            ptr_reg <= 8'h00;

        else
            if (ptr_we)
                ptr_reg <= ptr_new;
    end


//----------------------------------------------------------------
// ptr_logic
//----------------------------------------------------------------
always @*
    begin : ptr_logic
        ptr_new = 8'h00;
        ptr_we  = 1'b0;

        if (rst)
            begin
                ptr_new = 8'h00;
                ptr_we  = 1'b1;
            end

        if (cs)
            begin
                ptr_new = ptr_reg + 1'b1;
                ptr_we  = 1'b1;
            end
    end

endmodule // blockmem2r1wptr

          //======================================================================
          // EOF blockmem2r1wptr.v
          //======================================================================


//======================================================================
//
// montprod.v
// ---------
// Montgomery product calculator for the modular exponentiantion core.
//
// parameter OPW is operand word width in bits.
// parameter ADW is address width in bits.
//
//
// Author: Peter Magnusson, Joachim Strombergson
// Copyright (c) 2015, NORDUnet A/S All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//======================================================================

module montprod #(parameter OPW = 32, parameter ADW = 8)
       (
           input wire                  clk,
           input wire                  reset_n,

           input wire                  calculate,
           output wire                 ready,

           input wire [(ADW - 1) : 0]  length,

           output wire [(ADW - 1) : 0] opa_addr,
           input wire [(OPW - 1) : 0]  opa_data,

           output wire [(ADW - 1) : 0] opb_addr,
           input wire [(OPW - 1) : 0]  opb_data,

           output wire [(ADW - 1) : 0] opm_addr,
           input wire [(OPW - 1) : 0]  opm_data,

           output wire [(ADW - 1) : 0] result_addr,
           output wire [(OPW - 1) : 0] result_data,
           output wire                 result_we
       );


//----------------------------------------------------------------
// Internal constant and parameter definitions.
//----------------------------------------------------------------
localparam CTRL_IDLE            = 4'h0;
localparam CTRL_LOOP_ITER       = 4'h1;
localparam CTRL_LOOP_BQ         = 4'h2;
localparam CTRL_CALC_ADD        = 4'h3;
localparam CTRL_STALLPIPE_ADD   = 4'h4;
localparam CTRL_CALC_SDIV2      = 4'h5;
localparam CTRL_STALLPIPE_SDIV2 = 4'h6;
localparam CTRL_L_STALLPIPE_ES  = 4'h7;
localparam CTRL_EMIT_S          = 4'h8;

localparam SMUX_ZERO            = 2'h0;
localparam SMUX_ADD             = 2'h1;
localparam SMUX_SHR             = 2'h2;


//----------------------------------------------------------------
// Registers including update variables and write enable.
//----------------------------------------------------------------
reg                  ready_reg;
reg                  ready_new;
reg                  ready_we;

reg [3 : 0]          montprod_ctrl_reg;
reg [3 : 0]          montprod_ctrl_new;
reg                  montprod_ctrl_we;

reg  [1 : 0]         s_mux_new;
reg  [1 : 0]         s_mux_reg;

reg                  s_mem_we_reg;
reg                  s_mem_we_new;

reg [(ADW - 1) : 0]  s_mem_read_addr_reg;

reg                  q_new;
reg                  q_reg;
reg                  b_new;
reg                  b_reg;
reg                  bq_we;

reg [12 : 0]         loop_ctr_reg;
reg [12 : 0]         loop_ctr_new;
reg                  loop_ctr_we;
reg                  loop_ctr_set;
reg                  loop_ctr_dec;

reg [(13 - ADW - 1) : 0] b_bit_index_reg;
reg [(13 - ADW - 1) : 0] b_bit_index_new;
reg                      b_bit_index_we;

reg [(ADW - 1) : 0]  word_index_reg;
reg [(ADW - 1) : 0]  word_index_new;
reg                  word_index_we;
reg [(ADW - 1) : 0]  word_index_prev_reg;
reg                  reset_word_index_lsw;
reg                  reset_word_index_msw;
reg                  inc_word_index;
reg                  dec_word_index;

reg                  add_carry_in_sa_reg;
reg                  add_carry_in_sa_new;
reg                  add_carry_in_sm_reg;
reg                  add_carry_in_sm_new;

reg                  shr_carry_in_reg;
reg                  shr_carry_in_new;

reg                  first_iteration_reg;
reg                  first_iteration_new;
reg                  first_iteration_we;

reg                  test_reg;
reg                  test_new;

reg [(OPW - 2) : 0]  shr_data_out_reg;
reg                  shr_carry_out_reg;
reg                  shr_carry_out_new;

//----------------------------------------------------------------
// Wires.
//----------------------------------------------------------------
wire [(OPW - 1) : 0] add_result_sa;
wire                 add_carry_out_sa;
wire [(OPW - 1) : 0] add_result_sm;
wire                 add_carry_out_sm;
reg [(ADW - 1) : 0]  b_word_index; //loop counter as a word index
/* verilator lint_off UNOPTFLAT */
reg  [(OPW - 1) : 0] shr_data_in;
/* verilator lint_on UNOPTFLAT */
wire                 shr_carry_out;
wire [(OPW - 1) : 0] shr_data_out;

reg [(ADW - 1) : 0]  tmp_opa_addr;
reg                  tmp_result_we;

reg [(ADW - 1) : 0]  s_mem_read_addr;
wire [(OPW - 1) : 0] s_mem_read_data;
reg [(ADW - 1) : 0]  s_mem_write_addr;
reg [(OPW - 1) : 0]  s_mem_write_data;
reg [(OPW - 1) : 0]  tmp_s_mem_write_data;

reg [(OPW - 1) : 0]  sa_adder_data_in;
/* verilator lint_off UNOPTFLAT */
reg [(OPW - 1) : 0]  muxed_s_mem_read_data;
/* verilator lint_on UNOPTFLAT */
reg [(OPW - 1) : 0]  shifted_s_mem_write_data;

wire [(ADW - 1) : 0] length_m1;

// Temporary debug wires.
reg [1 : 0] state_trace;
reg [1 : 0] mux_trace;

//----------------------------------------------------------------
// Concurrent connectivity for ports etc.
//----------------------------------------------------------------
assign length_m1   = length - 1'b1;

assign opa_addr    = tmp_opa_addr;
assign opb_addr    = b_word_index;
assign opm_addr    = word_index_reg;

assign result_addr = word_index_prev_reg;
assign result_data = s_mem_read_data;
assign result_we   = tmp_result_we;

assign ready       = ready_reg;


//----------------------------------------------------------------
// Instantions
//----------------------------------------------------------------
blockmem1r1w #(.OPW(OPW), .ADW(ADW)) s_mem(
                 .clk(clk),
                 .read_addr(s_mem_read_addr),
                 .read_data(s_mem_read_data),
                 .wr(s_mem_we_reg),
                 .write_addr(s_mem_write_addr),
                 .write_data(s_mem_write_data)
             );

adder #(.OPW(OPW)) s_adder_sm(
          .a(muxed_s_mem_read_data),
          .b(opm_data),
          .carry_in(add_carry_in_sm_reg),
          .sum(add_result_sm),
          .carry_out(add_carry_out_sm)
      );


adder #(.OPW(OPW)) s_adder_sa(
          .a(sa_adder_data_in),
          .b(opa_data),
          .carry_in(add_carry_in_sa_reg),
          .sum(add_result_sa),
          .carry_out(add_carry_out_sa)
      );

shr #(.OPW(OPW)) shifter(
        .a(shr_data_in),
        .carry_in(shr_carry_in_reg),
        .adiv2(shr_data_out),
        .carry_out(shr_carry_out)
    );


//----------------------------------------------------------------
// reg_update
//
// Update functionality for all registers in the core.
// All registers are positive edge triggered with asynchronous
// active low reset. All registers have write enable.
//----------------------------------------------------------------
always @ (posedge clk or negedge reset_n)
    begin : reg_update
        if (!reset_n)
            begin
                test_reg            <= 1'b1;
                ready_reg           <= 1'b1;
                loop_ctr_reg        <= 13'h0;
                word_index_reg      <= {ADW{1'b0}};
                word_index_prev_reg <= {ADW{1'b0}};
                add_carry_in_sa_reg <= 1'b0;
                add_carry_in_sm_reg <= 1'b0;
                shr_data_out_reg    <= {(OPW - 1){1'b0}};
                shr_carry_in_reg    <= 1'b0;
                b_reg               <= 1'b0;
                q_reg               <= 1'b0;
                s_mux_reg           <= SMUX_ZERO;
                s_mem_we_reg        <= 1'b0;
                s_mem_read_addr_reg <= {ADW{1'b0}};
                b_bit_index_reg     <= {(13 - ADW){1'b0}};
                first_iteration_reg <= 1'b0;
                montprod_ctrl_reg   <= CTRL_IDLE;
            end
        else
            begin
                test_reg            <= test_new;

                s_mem_read_addr_reg <= s_mem_read_addr;
                s_mem_we_reg        <= s_mem_we_new;
                s_mux_reg           <= s_mux_new;

                word_index_prev_reg <= word_index_reg;

                shr_carry_in_reg    <= shr_carry_in_new;
                add_carry_in_sa_reg <= add_carry_in_sa_new;
                add_carry_in_sm_reg <= add_carry_in_sm_new;
                shr_data_out_reg    <= shr_data_out[(OPW - 2) : 0];

                if (word_index_we)
                    word_index_reg <= word_index_new;

                if (first_iteration_we)
                    first_iteration_reg <= first_iteration_new;

                if (b_bit_index_we)
                    b_bit_index_reg <= b_bit_index_new;

                if (bq_we)
                    begin
                        b_reg <= b_new;
                        q_reg <= q_new;
                    end

                if (ready_we)
                    ready_reg <= ready_new;

                if (loop_ctr_we)
                    loop_ctr_reg <= loop_ctr_new;

                if (montprod_ctrl_we)
                    begin
                        montprod_ctrl_reg <= montprod_ctrl_new;
                    end
            end
    end // reg_update


//----------------------------------------------------------------
// s_logic
//
// Logic to calculate S memory updates including address
// and write enable. This is the main montprod datapath.
//----------------------------------------------------------------
always @*
    begin : s_logic
        shr_carry_in_new      = 1'b0;
        muxed_s_mem_read_data = {OPW{1'b0}};
        sa_adder_data_in      = {OPW{1'b0}};
        add_carry_in_sa_new   = 1'b0;
        add_carry_in_sm_new   = 1'b0;
        s_mem_read_addr       = word_index_reg;
        s_mem_write_addr      = s_mem_read_addr_reg;
        s_mem_write_data      = {OPW{1'b0}};
        s_mem_we_new          = 1'b0;
        state_trace           = 0;
        mux_trace             = 0;
        tmp_s_mem_write_data  = {OPW{1'b0}};
        test_new              = 1'b0;
       // Added missing default statement to avoid latches!! Tony Duong 04/01/2020
        shr_data_in = 0;
       
        case (montprod_ctrl_reg)
            CTRL_LOOP_ITER:
                begin
                    s_mem_read_addr = length_m1;
                end

            CTRL_CALC_ADD:
                begin
                    //s = (s + q*M + b*A) >>> 1;, if(b==1) S+= A. Takes (1..length) cycles.
                    s_mem_we_new = b_reg | q_reg | first_iteration_reg;
                    state_trace  = 1;
                    test_new     = 1'b1;
                end

            CTRL_CALC_SDIV2:
                begin
                    //s = (s + q*M + b*A) >>> 1; s>>=1.  Takes (1..length) cycles.
                    s_mem_we_new = 1'b1;
                end

            default:
                begin
                end
        endcase


        case (s_mux_reg)
            SMUX_ADD:
                begin
                    mux_trace = 1;

                    if (first_iteration_reg)
                        muxed_s_mem_read_data = {OPW{1'b0}};
                    else
                        muxed_s_mem_read_data = s_mem_read_data;


                    if (q_reg)
                        sa_adder_data_in = add_result_sm;
                    else
                        sa_adder_data_in = muxed_s_mem_read_data;


                    if (b_reg)
                        tmp_s_mem_write_data = add_result_sa;
                    else if (q_reg)
                        tmp_s_mem_write_data = add_result_sm;
                    else if (first_iteration_reg)
                        tmp_s_mem_write_data = {OPW{1'b0}};

                    s_mem_write_data = tmp_s_mem_write_data;
                    add_carry_in_sa_new = add_carry_out_sa;
                    add_carry_in_sm_new = add_carry_out_sm;


                    // Experimental integration of shift in add.
                    shr_data_in              = tmp_s_mem_write_data;
                    shifted_s_mem_write_data = {shr_carry_out, shr_data_out_reg};
                end


            SMUX_SHR:
                begin
                    shr_data_in      = s_mem_read_data;
                    s_mem_write_data = shr_data_out;
                    shr_carry_in_new = shr_carry_out;
                end

            default:
                begin
                end
        endcase
    end // s_logic


//----------------------------------------------------------------
// bq
//
// Extract b and q bits.
// b: current bit of B used.
// q = (s - b * A) & 1
// update the b bit and word indices based on loop counter.
//----------------------------------------------------------------
always @*
    begin : bq
        b_new = opb_data[b_bit_index_reg];

        if (first_iteration_reg)
            q_new = 1'b0 ^ (opa_data[0] & b_new);
        else
            q_new = s_mem_read_data[0] ^ (opa_data[0] & b_new);

        // B_bit_index      = 5'h1f - loop_counter[4:0];
        b_bit_index_new = ((2**(13 - ADW)) - 1'b1) - loop_ctr_reg[(13 - ADW - 1) : 0];
        b_word_index    = loop_ctr_reg[12 : (13 - ADW)];
    end // bq


//----------------------------------------------------------------
// word_index
//
// Logic that implements the word index used to drive
// addresses for operands.
//----------------------------------------------------------------
always @*
    begin : word_index
        word_index_new = {ADW{1'b0}};
        word_index_we  = 1'b0;

        if (reset_word_index_lsw)
            begin
                word_index_new = length_m1;
                word_index_we  = 1'b1;
            end

        if (reset_word_index_msw)
            begin
                word_index_new = {ADW{1'b0}};
                word_index_we  = 1'b1;
            end

        if (inc_word_index)
            begin
                word_index_new = word_index_reg + 1'b1;
                word_index_we  = 1'b1;
            end

        if (dec_word_index)
            begin
                word_index_new = word_index_reg - 1'b1;
                word_index_we  = 1'b1;
            end
    end // word_index


//----------------------------------------------------------------
// loop_ctr
// Logic for updating the loop counter.
//----------------------------------------------------------------
always @*
    begin : loop_ctr
        loop_ctr_new = 13'h0;
        loop_ctr_we  = 1'b0;

        if (loop_ctr_set)
            begin
                loop_ctr_new = {length, {(13 - ADW){1'b0}}} - 1'b1;
                loop_ctr_we  = 1'b1;
            end

        if (loop_ctr_dec)
            begin
                loop_ctr_new = loop_ctr_reg - 1'b1;
                loop_ctr_we  = 1'b1;
            end
    end


//----------------------------------------------------------------
// montprod_ctrl
//
// Control FSM for the montgomery product calculator.
//----------------------------------------------------------------
always @*
    begin : montprod_ctrl
        ready_new            = 1'b0;
        ready_we             = 1'b0;
        loop_ctr_set         = 1'b0;
        loop_ctr_dec         = 1'b0;
        b_bit_index_we       = 1'b0;
        bq_we                = 1'b0;
        s_mux_new            = SMUX_ZERO;
        dec_word_index       = 1'b0;
        inc_word_index       = 1'b0;
        reset_word_index_lsw = 1'b0;
        reset_word_index_msw = 1'b0;
        first_iteration_new  = 1'b0;
        first_iteration_we   = 1'b0;
        tmp_opa_addr         = word_index_reg;
        tmp_result_we        = 1'b0;
        montprod_ctrl_new    = CTRL_IDLE;
        montprod_ctrl_we     = 1'b0;


        case (montprod_ctrl_reg)
            CTRL_IDLE:
                begin
                    if (calculate)
                        begin
                            first_iteration_new  = 1'b1;
                            first_iteration_we   = 1'b1;
                            ready_new            = 1'b0;
                            ready_we             = 1'b1;
                            reset_word_index_lsw = 1'b1;
                            loop_ctr_set         = 1'b1;
                            montprod_ctrl_new    = CTRL_LOOP_ITER;
                            montprod_ctrl_we     = 1'b1;
                        end
                end

            //calculate q = (s - b * A) & 1;.
            // Also abort loop if done.
            CTRL_LOOP_ITER:
                begin
                    tmp_opa_addr         = length_m1;
                    b_bit_index_we       = 1'b1;
                    montprod_ctrl_new    = CTRL_LOOP_BQ;
                    montprod_ctrl_we     = 1'b1;
                end

            CTRL_LOOP_BQ:
                begin
                    reset_word_index_lsw = 1'b1;
                    bq_we                = 1'b1;
                    montprod_ctrl_new    = CTRL_CALC_ADD;
                    montprod_ctrl_we     = 1'b1;
                end

            CTRL_CALC_ADD:
                begin
                    s_mux_new      = SMUX_ADD;

                    if (word_index_reg == 0)
                        begin
                            reset_word_index_lsw = 1'b1;
                            montprod_ctrl_new    = CTRL_STALLPIPE_ADD;
                            montprod_ctrl_we     = 1'b1;
                        end
                    else
                        begin
                            dec_word_index = 1'b1;
                        end
                end

            CTRL_STALLPIPE_ADD:
                begin
                    first_iteration_new  = 1'b0;
                    first_iteration_we   = 1'b1;
                    reset_word_index_msw = 1'b1;
                    montprod_ctrl_new    = CTRL_CALC_SDIV2;
                    montprod_ctrl_we     = 1'b1;
                end

            CTRL_CALC_SDIV2:
                begin
                    s_mux_new      = SMUX_SHR;

                    if (word_index_reg == length_m1)
                        begin
                            montprod_ctrl_new = CTRL_STALLPIPE_SDIV2;
                            montprod_ctrl_we  = 1'b1;
                        end
                    else
                        inc_word_index = 1'b1;
                end

            CTRL_STALLPIPE_SDIV2:
                begin
                    loop_ctr_dec         = 1'b1;
                    montprod_ctrl_new    = CTRL_LOOP_ITER;
                    montprod_ctrl_we     = 1'b1;
                    reset_word_index_lsw = 1'b1;

                    if (loop_ctr_reg == 0)
                        begin
                            montprod_ctrl_new = CTRL_L_STALLPIPE_ES;
                            montprod_ctrl_we  = 1'b1;
                        end
                end

            CTRL_L_STALLPIPE_ES:
                begin
                    montprod_ctrl_new = CTRL_EMIT_S;
                    montprod_ctrl_we  = 1'b1;
                end

            CTRL_EMIT_S:
                begin
                    dec_word_index = 1'b1;
                    tmp_result_we  = 1'b1;

                    if (word_index_prev_reg == 0)
                        begin
                            ready_new         = 1'b1;
                            ready_we          = 1'b1;
                            montprod_ctrl_new = CTRL_IDLE;
                            montprod_ctrl_we  = 1'b1;
                        end
                end

            default:
                begin
                end

        endcase // case (montprod_ctrl_reg)
    end // montprod_ctrl

endmodule // montprod

          //======================================================================
          // EOF montprod.v
          //======================================================================


//======================================================================
//
// residue.v
// ---------
// Modulus 2**2N residue calculator for montgomery calculations.
//
// m_residue_2_2N_array( N, M, Nr)
//   Nr = 00...01 ; Nr = 1 == 2**(2N-2N)
//   for (int i = 0; i < 2 * N; i++)
//     Nr = Nr shift left 1
//     if (Nr less than M) continue;
//     Nr = Nr - M
// return Nr
//
//
//
// Author: Peter Magnusson
// Copyright (c) 2015, NORDUnet A/S All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//======================================================================

module residue #(parameter OPW = 32, parameter ADW = 8)
       (
           input wire                  clk,
           input wire                  reset_n,

           input wire                  calculate,
           output wire                 ready,

           input wire  [14 : 0]        nn, //MAX(2*N)=8192*2 (14 bit)
           input wire  [(ADW - 1) : 0] length,

           output wire [(ADW - 1) : 0] opa_rd_addr,
           input wire  [(OPW - 1) : 0] opa_rd_data,
           output wire [(ADW - 1) : 0] opa_wr_addr,
           output wire [(OPW - 1) : 0] opa_wr_data,
           output wire                 opa_wr_we,

           output wire [(ADW - 1) : 0] opm_addr,
           input wire  [(OPW - 1) : 0] opm_data
       );

//----------------------------------------------------------------
// Internal constant and parameter definitions.
//----------------------------------------------------------------
localparam CTRL_IDLE          = 4'h0;
localparam CTRL_INIT          = 4'h1;
localparam CTRL_INIT_STALL    = 4'h2;
localparam CTRL_SHL           = 4'h3;
localparam CTRL_SHL_STALL     = 4'h4;
localparam CTRL_COMPARE       = 4'h5;
localparam CTRL_COMPARE_STALL = 4'h6;
localparam CTRL_SUB           = 4'h7;
localparam CTRL_SUB_STALL     = 4'h8;
localparam CTRL_LOOP          = 4'h9;


//----------------------------------------------------------------
// Registers including update variables and write enable.
//----------------------------------------------------------------
reg [(ADW - 1) : 0]  opa_rd_addr_reg;
reg [(ADW - 1) : 0]  opa_wr_addr_reg;
reg [(OPW - 1) : 0]  opa_wr_data_reg;
reg                  opa_wr_we_reg;
reg [(ADW - 1) : 0]  opm_addr_reg;
reg                  ready_reg;
reg                  ready_new;
reg                  ready_we;
reg [03 : 0]         residue_ctrl_reg;
reg [03 : 0]         residue_ctrl_new;
reg                  residue_ctrl_we;
reg                  reset_word_index;
reg                  reset_n_counter;
reg [14 : 0]         loop_counter_1_to_nn_reg; //for i = 1 to nn (2*N)
reg [14 : 0]         loop_counter_1_to_nn_new;
reg                  loop_counter_1_to_nn_we;
reg [14 : 0]         nn_reg;
reg                  nn_we;
reg [(ADW - 1) : 0]  length_m1_reg;
reg [(ADW - 1) : 0]  length_m1_new;
reg                  length_m1_we;
reg [(ADW - 1) : 0]  word_index_reg;
reg [(ADW - 1) : 0]  word_index_new;
reg                  word_index_we;

reg  [(OPW - 1) : 0] one_data;
wire [(OPW - 1) : 0] sub_data;
wire [(OPW - 1) : 0] shl_data;
reg                  sub_carry_in_new;
reg                  sub_carry_in_reg;
wire                 sub_carry_out;
reg                  shl_carry_in_new;
reg                  shl_carry_in_reg;
wire                 shl_carry_out;


//----------------------------------------------------------------
// Concurrent connectivity for ports etc.
//----------------------------------------------------------------
assign opa_rd_addr = opa_rd_addr_reg;
assign opa_wr_addr = opa_wr_addr_reg;
assign opa_wr_data = opa_wr_data_reg;
assign opa_wr_we   = opa_wr_we_reg;
assign opm_addr    = opm_addr_reg;
assign ready       = ready_reg;


//----------------------------------------------------------------
// Instantions
//----------------------------------------------------------------
adder #(.OPW(OPW)) add_inst(
          .a(opa_rd_data),
          .b( ~ opm_data),
          .carry_in(sub_carry_in_reg),
          .sum(sub_data),
          .carry_out(sub_carry_out)
      );

shl #(.OPW(OPW)) shl_inst(
        .a(opa_rd_data),
        .carry_in(shl_carry_in_reg),
        .amul2(shl_data),
        .carry_out(shl_carry_out)
    );


//----------------------------------------------------------------
// reg_update
//----------------------------------------------------------------
always @ (posedge clk or negedge reset_n)
    begin
        if (!reset_n)
            begin
                residue_ctrl_reg         <= CTRL_IDLE;
                word_index_reg           <= {ADW{1'b1}};
                length_m1_reg            <= {ADW{1'b1}};
                nn_reg                   <= 15'h0;
                loop_counter_1_to_nn_reg <= 15'h0;
                ready_reg                <= 1'b1;
                sub_carry_in_reg         <= 1'b0;
                shl_carry_in_reg         <= 1'b0;
            end
        else
            begin
                if (residue_ctrl_we)
                    residue_ctrl_reg <= residue_ctrl_new;

                if (word_index_we)
                    word_index_reg <= word_index_new;

                if (length_m1_we)
                    length_m1_reg <= length_m1_new;

                if (nn_we)
                    nn_reg <= nn;

                if (loop_counter_1_to_nn_we)
                    loop_counter_1_to_nn_reg <= loop_counter_1_to_nn_new;

                if (ready_we)
                    ready_reg <= ready_new;

                sub_carry_in_reg <= sub_carry_in_new;
                shl_carry_in_reg <= shl_carry_in_new;
            end
    end // reg_update


//----------------------------------------------------------------
// loop counter process. implements for (int i = 0; i < 2 * N; i++)
//
// m_residue_2_2N_array( N, M, Nr)
//   Nr = 00...01 ; Nr = 1 == 2**(2N-2N)
//   for (int i = 0; i < 2 * N; i++)
//     Nr = Nr shift left 1
//     if (Nr less than M) continue;
//     Nr = Nr - M
// return Nr
//
//----------------------------------------------------------------
always @*
    begin : process_1_to_2n
        loop_counter_1_to_nn_new = loop_counter_1_to_nn_reg + 15'h1;
        loop_counter_1_to_nn_we  = 1'b0;

        if (reset_n_counter)
            begin
                loop_counter_1_to_nn_new = 15'h1;
                loop_counter_1_to_nn_we  = 1'b1;
            end

        if (residue_ctrl_reg == CTRL_LOOP)
            loop_counter_1_to_nn_we  = 1'b1;
    end


//----------------------------------------------------------------
// implements looping over words in a multiword operation
//----------------------------------------------------------------
always @*
    begin : word_index_process
        word_index_new = word_index_reg - 1'b1;
        word_index_we  = 1'b1;

        if (reset_word_index)
            word_index_new = length_m1_reg;

        if (residue_ctrl_reg == CTRL_IDLE)
            //reduce a pipeline stage with early read
            word_index_new = length_m1_new;
    end


//----------------------------------------------------------------
// writer process. implements:
//   Nr = 00...01 ; Nr = 1 == 2**(2N-2N)
//   Nr = Nr shift left 1
//   Nr = Nr - M
//
// m_residue_2_2N_array( N, M, Nr)
//   Nr = 00...01 ; Nr = 1 == 2**(2N-2N)
//   for (int i = 0; i < 2 * N; i++)
//     Nr = Nr shift left 1
//     if (Nr less than M) continue;
//     Nr = Nr - M
// return Nr
//----------------------------------------------------------------
always @*
    begin : writer_process
        opa_wr_addr_reg = word_index_reg;
        case (residue_ctrl_reg)
            CTRL_INIT:
                begin
                    opa_wr_data_reg = one_data;
                    opa_wr_we_reg   = 1'b1;
                end

            CTRL_SUB:
                begin
                    opa_wr_data_reg = sub_data;
                    opa_wr_we_reg   = 1'b1;
                end

            CTRL_SHL:
                begin
                    opa_wr_data_reg = shl_data;
                    opa_wr_we_reg   = 1'b1;
                end

            default:
                begin
                    opa_wr_data_reg = 32'h0;
                    opa_wr_we_reg   = 1'b0;
                end
        endcase
    end

//----------------------------------------------------------------
// reader process. reads from new value because it occurs one
// cycle earlier than the writer.
//----------------------------------------------------------------
always @*
    begin : reader_process
        opa_rd_addr_reg = word_index_new;
        opm_addr_reg    = word_index_new;
    end


//----------------------------------------------------------------
// carry process. "Ripple carry awesomeness!"
//----------------------------------------------------------------
always @*
    begin : carry_process
        case (residue_ctrl_reg)
            CTRL_COMPARE:
                sub_carry_in_new = sub_carry_out;
            CTRL_SUB:
                sub_carry_in_new = sub_carry_out;
            default:
                sub_carry_in_new = 1'b1;
        endcase

        case (residue_ctrl_reg)
            CTRL_SHL:
                shl_carry_in_new = shl_carry_out;
            default:
                shl_carry_in_new = 1'b0;
        endcase
    end


//----------------------------------------------------------------
// Nr = 00...01 ; Nr = 1 == 2**(2N-2N)
//----------------------------------------------------------------
always @*
    begin : one_process
        one_data = 32'h0;
        if (residue_ctrl_reg == CTRL_INIT)
            if (word_index_reg == length_m1_reg)
                one_data = {{(OPW - 1){1'b0}}, 1'b1};
    end


//----------------------------------------------------------------
// residue_ctrl
//
// Control FSM for residue
//----------------------------------------------------------------
always @*
    begin : residue_ctrl
        ready_new        = 1'b0;
        ready_we         = 1'b0;
        reset_word_index = 1'b0;
        reset_n_counter  = 1'b0;
        length_m1_new    = length - 1'b1;
        length_m1_we     = 1'b0;
        nn_we            = 1'b0;
        residue_ctrl_new = CTRL_IDLE;
        residue_ctrl_we  = 1'b0;

        case (residue_ctrl_reg)
            CTRL_IDLE:
                if (calculate)
                    begin
                        ready_new        = 1'b0;
                        ready_we         = 1'b1;
                        reset_word_index = 1'b1;
                        length_m1_we     = 1'b1;
                        nn_we            = 1'b1;
                        residue_ctrl_new = CTRL_INIT;
                        residue_ctrl_we  = 1'b1;
                    end

            // Nr = 00...01 ; Nr = 1 == 2**(2N-2N)
            CTRL_INIT:
                if (word_index_reg == 0)
                    begin
                        residue_ctrl_new = CTRL_INIT_STALL;
                        residue_ctrl_we  = 1'b1;
                    end

            CTRL_INIT_STALL:
                begin
                    reset_word_index = 1'b1;
                    reset_n_counter  = 1'b1;
                    residue_ctrl_new = CTRL_SHL;
                    residue_ctrl_we  = 1'b1;
                end

            // Nr = Nr shift left 1
            CTRL_SHL:
                begin
                    if (word_index_reg == 0)
                        begin
                            residue_ctrl_new = CTRL_SHL_STALL;
                            residue_ctrl_we  = 1'b1;
                        end
                end

            CTRL_SHL_STALL:
                begin
                    reset_word_index = 1'b1;
                    residue_ctrl_new = CTRL_COMPARE;
                    residue_ctrl_we  = 1'b1;
                end

            //if (Nr less than M) continue
            CTRL_COMPARE:
                if (word_index_reg == 0)
                    begin
                        residue_ctrl_new = CTRL_COMPARE_STALL;
                        residue_ctrl_we  = 1'b1;
                    end

            CTRL_COMPARE_STALL:
                begin
                    reset_word_index = 1'b1;
                    residue_ctrl_we  = 1'b1;
                    if (sub_carry_in_reg == 1'b1)
                        //TODO: Bug! detect CF to detect less than, but no detect ZF to detect equal to.
                        residue_ctrl_new = CTRL_SUB;
                    else
                        residue_ctrl_new = CTRL_LOOP;
                end

            //Nr = Nr - M
            CTRL_SUB:
                if (word_index_reg == 0)
                    begin
                        residue_ctrl_new = CTRL_SUB_STALL;
                        residue_ctrl_we  = 1'b1;
                    end

            CTRL_SUB_STALL:
                begin
                    residue_ctrl_new = CTRL_LOOP;
                    residue_ctrl_we  = 1'b1;
                end

            //for (int i = 0; i < 2 * N; i++)
            CTRL_LOOP:
                begin
                    if (loop_counter_1_to_nn_reg == nn_reg)
                        begin
                            ready_new = 1'b1;
                            ready_we  = 1'b1;
                            residue_ctrl_new = CTRL_IDLE;
                            residue_ctrl_we  = 1'b1;
                        end
                    else
                        begin
                            reset_word_index = 1'b1;
                            residue_ctrl_new = CTRL_SHL;
                            residue_ctrl_we  = 1'b1;
                        end
                end

            default:
                begin
                end

        endcase
    end

endmodule // residue

          //======================================================================
          // EOF residue.v
          //======================================================================


//======================================================================
//
// shl.v
// -----
// One bit left shift of words with carry in and carry out. Used in
// the residue module of the modexp core.
//
//
// Author: Peter Magnusson, Joachim Strömbergson
// Copyright (c) 2015, NORDUnet A/S All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//======================================================================

module shl #(parameter OPW = 32)
       (
           input  wire [(OPW - 1) : 0] a,
           input  wire                 carry_in,

           output wire [(OPW - 1) : 0] amul2,
           output wire                 carry_out
       );

assign amul2     = {a[(OPW - 2) : 0], carry_in};
assign carry_out = a[(OPW - 1)];

endmodule // shl

          //======================================================================
          // EOF shl.v
          //======================================================================


//======================================================================
//
// shr32.v
// -------
// One bit right shift with carry in and carry out.
// Used in the montprod module of the modexp core.
//
//
// Author: Peter Magnusson, Joachim Strömbergson
// Copyright (c) 2015, NORDUnet A/S All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// - Neither the name of the NORDUnet nor the names of its contributors may
//   be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//======================================================================

module shr #(parameter OPW = 32)
       (
           input wire  [(OPW - 1) : 0] a,
           input wire                  carry_in,

           output wire [(OPW - 1) : 0] adiv2,
           output wire                 carry_out
       );

assign adiv2      = {carry_in, a[(OPW - 1) : 1]};
assign carry_out = a[0];

endmodule // shr

          //======================================================================
          // EOF shr.v
          //======================================================================
