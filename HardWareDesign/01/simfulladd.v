module simfulladd;
   wire [1:0] sf;
   wire [1:0] ss;
   reg 	x,y,cin;
   
   initial begin
      $monitor ( "%t %b(x) + %b(y) + %b(cin)= %b(FAF) = %b(FAS)", $time, x, y, cin, sf, ss);
      x <= 0; y <= 0; cin <= 0;
      #40 x <= 0; y <= 1; cin <= 0;
      #40 x <= 1; y <= 0; cin <= 0;
      #40 x <= 1; y <= 1; cin <= 0;
      #40 x <= 0; y <= 0; cin <= 1;
      #40 x <= 0; y <= 1; cin <= 1;
      #40 x <= 1; y <= 0; cin <= 1;
      #40 x <= 1; y <= 1; cin <= 1;  
      #40
	$finish;
   end
   FullAdderFunction add(x,y,cin,sf[1],sf[0]);
   FullAdderStructure add1(x,y,cin,ss[1],ss[0]);
   
   
endmodule // simfulladd
      
      
      
