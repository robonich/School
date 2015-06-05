module mul(A, B, O, ck, start, fin);
   input [7:0] A, B;
   output [16:0] O;
   input 	 ck, start;
   output 	 fin;
   reg [7:0] 	 AIN, BIN;
   reg [3:0] 	 ST;
   reg [16:0] 	 O;
   reg 		 fin;
   
   
   //reg [16:0] 	 OR;
   
   always @(posedge ck) begin
      AIN <= A;
      BIN <= B;
      
      if(start == 1) begin
	 ST <= 0;
	 O <= 0;
	 
      end else begin
	 if(ST < 7) begin
	    O <= (O<<1) + AIN * BIN[7-ST];
	    fin <= 0;
	    ST <= ST + 1;
	 end else if(ST == 7) begin
	    O <= (O<<1) + AIN * BIN[0];
	    fin <= 1;
	    ST <= ST + 1;
	 end else if(ST == 8) begin
	    fin <= 0;
	 end
      end // else: !if(start == 1)
   end // always @ (posedge ck)
endmodule // mul

   
      
		       
