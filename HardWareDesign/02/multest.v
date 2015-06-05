module  multest;
   reg	[7:0]	A, B;
   reg 		ck;
   reg 		start;
   reg [3:0] 	st;
   wire [16:0] 	O;
   reg [16:0] 	OR;
   initial begin
      ck=0;
      start=0;
      st=0;
      $dumpvars;
      $dumpfile("mul.vcd");
      $monitor( "%t\tA=%h, B=%h, (OUT=%h) OUT=%h", $time, A, B, O, OR );
      #1000  $finish;
   end
   
   mul	MUL(A , B , O ,  ck, start,fin);
   always	#10	ck = ~ck;
   always @(negedge ck) begin
      if( st == 0 ) start <= 1;
      else start <= 0;
      if( fin == 1 ) OR <= O;
      st <= st+1;
      A = $random;
      B = $random;
   end
   
endmodule // multest
