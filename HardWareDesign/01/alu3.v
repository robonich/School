module alu22(A, B, O, CTR, ck);
   input [7:0] A, B;
   output [7:0] O;
   input [3:0] CTR;
   input 	ck;
   reg [7:0] 	INA, INB, O;
   reg [3:0] 	C;
   wire [7:0] 	OUT;

   function [7:0] alufunc;
      input [7:0] A;
      input [7:0] B;
      input [3:0] C;
      case(C)
	'b0000: alufunc <= INA + INB;
	'b0001: alufunc <= INA - INB;
	'b1001: alufunc <= INA | INB;
	'b1010: alufunc <= INA ^ INB;
	'b1011: alufunc <= ~INA;
	'b1100: alufunc <= INA >> 1;
	'b1101: alufunc <= INA << 1;
	'b1110: alufunc <= {INA[0], INA[7:1]};
	'b1111: alufunc <= {INA[6:0], INA[7]};
      endcase // case (C)
   endfunction // case
      
   always @(posedge ck) begin
      C <= CTR;
      INA <= A;
      INB <= B;
      O <= function(INA, INB, C);
endmodule // alu22



