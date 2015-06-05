module top;
   reg [7:0] data_in;
   reg 	     ck, rst, crc_en;
   wire [4:0] crc_out;

   crc crc(data_in, crc_en, crc_out, rst, ck);

   initial begin
      ck=0; rst=0;
      $dumpfile("crc.vcd");
      $dumpvars;
      #100 rst=1;
      #100 rst=0;
      #1000 $finish;
   end

   always #10 ck<=~ck;
   always @(negedge ck) begin
      crc_en <= $random;
      data_in <= $random;
   end

endmodule
