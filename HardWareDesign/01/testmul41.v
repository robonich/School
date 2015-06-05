module testmul4;
   wire [5:0] s;//符号付き
   reg [3:0]  a, b;
   initial begin
      $monitor( "%t %b * %b = %b", $time, a, b, s);
      a <= 0; b <= 0;
      #40 a <= 1; b <= 3;
      #40 a <= 0; b <= 2;
      #40 a <= 4; b <= 8;
      #40 a <= 8; b <= 4;
      #40 a <= $random; b <= $random;
      #40 a <= $random; b <= $random;
      #40
	$finish;      
   end
   mul4 mul( s,a,b );
endmodule

