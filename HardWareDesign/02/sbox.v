module sbox(in, s, ck);
   input [3:0] in;
   output [3:0] s;
   input 	ck;
   reg [3:0] 	IN;
   reg [3:0] 	s;

   always @(posedge ck) begin
      
      case(IN)
	'b0000 : s <= 14;
	'b0001 : s <= 4;
	'b0010 : s <= 13;
	'b0011 : s <= 1;
	'b0100 : s <= 0;
	'b0101 : s <= 15;
	'b0110 : s <= 7;
	'b0111 : s <= 4;
	'b1000 : s <= 4;
	'b1001 : s <= 1;
	'b1010 : s <= 14;
	'b1011 : s <= 8;
	'b1100 : s <= 15;
	'b1101 : s <= 12;
	'b1110 : s <= 8;
	'b1111 : s <= 2;
      endcase // case (IN)
      
   end // always @ (posedge ck)
   

endmodule
