module simfulladd;
   wire s,cout;
   reg 	x,y,cin;
   initial begin
      $monitor ( "%t %b + %b + %b = %b = %b", $time, x, y, cin, cout, s);
      x <= 0;
      y <= 0;
      #40 x <= 0; y <= 0; cin <= 0;
      #40 x <= 1; y <= 1; cin <= 0;
      #40 x <= $random; y <= $random; cin <= $random;
      #40
	$finish;
   end
   FullAdderStructure add(x,y,cin,cout,s);
endmodule // simfulladd

      
      
      
