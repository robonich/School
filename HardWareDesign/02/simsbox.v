module top;
   reg [3:0] in;
   reg 	     ck;
   wire [3:0] s;

   sbox sbox(in,s,ck);
   initial begin
      ck=0;
      $dumpfile("sbox.vcd");
      $dumpvars;
      #1000 $finish;
   end

   always #10 ck <= ~ck;
   always @(negedge ck) begin
      in = $random;
   end

endmodule

