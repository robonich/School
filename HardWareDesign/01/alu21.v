module alu21(A, B, O, CTR, ck);
   input [7:0] A, B;
   output [7:0] O;
   input [3:0] CTR;
   input 	ck;
   reg [7:0] 	INA, INB O;
   reg [3:0] 	C;
   wire [7:0] 	OUT;
   always @(posedge ck) begin
      INA <= A;
      INB <= B;
      C <= CTR;
      O <= OUT;
   end
   
   always @(C or INA or INB) begin
      case(C)
	'b0000: OUT <= INA + INB;
	'b0001: OUT <= INA - INB;
	'b1001: OUT <= INA | INB;
	'b1010: OUT <= INA ^ INB;
	'b1011: OUT <= ~INA;
	'b1100: OUT <= INA >> 1;
	'b1101: OUT <= INA << 1;
	'b1110: OUT <= {INA[0], INA[7:1]};
	'b1111: OUT <= {INA[6:0], INA[7]};
      endcase // case (C)
   end // always @ (C or INA or INB)
endmodule // alu21

