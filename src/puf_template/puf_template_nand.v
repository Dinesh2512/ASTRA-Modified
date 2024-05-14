
// Generated by Cadence Genus(TM) Synthesis Solution 19.15-s090_1
// Generated on: Jun 21 2022 11:20:11 EDT (Jun 21 2022 15:20:11 UTC)

// Verification Directory fv/puf_nand_template 

module puf_template_nand(in0, in1, in2, in3, sel, out0, out1);
  input in0, in1, in2, in3, sel;
  output out0, out1;
  wire in0, in1, in2, in3, sel;
  wire out0, out1;
  wire w0,w1,w2,w3,w4,w5,w6,w7;

  NAND2X1 inst1 (.A (w0), .B (w1), .Y (out0));
  NAND2X1 inst2 (.A (w2), .B (w3), .Y (out1));
  
  MUX2X1 inst4 ( .A(in0), .B(out1), .S(sel), .Y(w4) );
  INVX1 inst5 ( .A(w4), .Y(w0) );
  MUX2X1 inst6 ( .A(in1), .B(out1), .S(sel), .Y(w5) );
  INVX1 inst7 ( .A(w5), .Y(w1) );

  MUX2X1 inst8 ( .A(in2), .B(out0), .S(sel), .Y(w6) );
  INVX1 inst9 ( .A(w6), .Y(w2) );
  MUX2X1 inst10 ( .A(in3), .B(out0), .S(sel), .Y(w7) );
  INVX1 inst11 ( .A(w7), .Y(w3) );

endmodule

