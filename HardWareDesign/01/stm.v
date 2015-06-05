module stm(ck, rst, x);
   output [3:0] x;
   input ck, rst;
   reg 	 st;
   reg [3:0] x;
   always @(posedge ck) begin
      if(rst) begin
	 st <= 0;
	 x <= 0;
      end else begin
	 if(st == 0) begin
	    if(x == 0) st <= 1;
	    else x <= x - 1;
	 end else begin
	    if(x == 15) st <= 0;
	    else x <= x + 1;
	 end
      end // else: !if(rst)
   end // always @ (posedge ck)
endmodule // stm
