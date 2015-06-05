module alutest;
   reg [7:0] A, B;
   reg [3:0] CTR;
   reg 	     ck;
   wire [7:0] O;
   initial begin
      ck = 0;
      $monitor("%t\tA=%h, B=%h, CTR=%h, OUT=%h", $time, A, B, CTR, O);
      #1000 $finish;

   end
   alu ALU(A, B, O, CTR, ck);
   always #10 ck = ~ck;
   always @(negedge ck) begin
      A = $random;
      B = $random;
      CTR = $random;
   end
endmodule // alutest

     
