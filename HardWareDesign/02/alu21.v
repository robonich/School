module  alu(A,B,O,CTR,ck);
   input	[7:0]	A, B;
   input [3:0] 		CTR;
   input 		ck;
   output [7:0] 	O;
   reg [7:0] 		INA, INB, OUT, O;
   reg [3:0] 		C;

   always @(posedge ck) begin
      C <= CTR;	
      INA <= A;
      INB <= B;
      O <= OUT;
   end
   
   always @(C or INA or INB) begin
      case  (C)
	'b0000 : OUT <= INA + INB;
	'b0001 : OUT <= INA - INB;
	'b1000 : OUT <= INA & INB;
	'b1001 : OUT <= INA | INB;
	'b1010 : OUT <= INA ^ INB;
	'b1011 : OUT <= ~INA;
	'b1100 : OUT <= INA>>1;
	'b1101 : OUT <= INA<<1;
	'b1110 : OUT <= {INA[0], INA[7:1]};
	'b1111 : OUT <= {INA[6:0], INA[7]};
      endcase // case (C)
 
   end // always @ (C or INA or INB)
   
endmodule // alu
