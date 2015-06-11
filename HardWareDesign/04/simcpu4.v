module simcpu;
reg CK, RST;
wire RW;
wire [15:0] IA, DA, DD;
reg [15:0] ID, DDi;
reg [15:0] IMEM [0:127], DMEM[0:127];

	CPU c(CK,RST,IA,ID,DA,DD,RW);

   assign DD = ((RW == 1) ? DDi : 'b Z);//Loadのとき、DDi=DMEM[DA]が読みこまれる

initial begin
   $dumpfile("cpu4.vcd");
   $dumpvars;
	CK = 0;
	RST = 0;
#5	RST = 1;
#100	RST = 0;
	
        #10000 $finish;
end

always @(negedge CK) begin
   if( DA == 'b 0010 && RW == 0 ) begin //DAはcpuから与えられる。DDはinout。RWはcpu与えられる。
      $display( "%d * %d = %d, time = %t", DMEM[0], DMEM[1], DD, $time );
      $finish;
   end
end

always @(negedge CK) begin
	ID = IMEM[IA];//IAはcpuから与えられる命令の番地
end

always @(negedge CK) begin
   if( RW == 1 ) DDi = DMEM[DA];//loadのとき、DDiが更新される。
   else DMEM[DA] = DD;//storeのときDMEM[DA]がDDによって更新される。DMEM[DA]はデータバス
end

initial begin
   

   DMEM[0]= 'd 2;
   DMEM[1]= 'd 255;
   DMEM[2]= 'b 0000_0000_0000_0000;

   IMEM[0]= 'b 1100_0000_0000_0000; // IMM, R0, [0]
   IMEM[1]= 'b 1100_0001_0000_0001; // IMM, R1, [1]
   IMEM[2]= 'b 1100_0010_0000_0010; // IMM, R2, [2]

   IMEM[5]= 'b 1011_0101_0000_0000; // load, R5 = D[R0]
   IMEM[6]= 'b 1011_0110_0000_0001; // load, R6 = D[R1]

   IMEM[7]= 'b 1100_0111_0000_0001; // IMM, R7, [00000001] 桁数用
   IMEM[8]= 'b 1100_1000_0000_0000; // IMM, R8. [0] 一時値保管用
   IMEM[9]= 'b 1100_1001_0000_1111; // IMM, R9, [15] if1
   IMEM[10]='b 1100_1010_0001_0011; // IMM, R10, [19], if2
   IMEM[11]='b 1100_1011_0000_1100; // IMM, R11, [12], jump
   IMEM[12]='b 0101_1100_0110_0111; //　論理和, R12 = R6 & R7
   IMEM[13]='b 1001_0000_0000_1001; // if(上==0) PC = 15 = R9
   IMEM[14]='b 0000_1000_1000_0101; // R8 = R8+R5
   IMEM[15]='b 0011_0101_0101_0111; // R5 = R5 << R7
   IMEM[16]='b 0010_0110_0110_0111; // R6 = R6 >> R7
   IMEM[17]='b 1001_0000_0000_1010; // if(上==0) PC = 19 = R10
   IMEM[18]='b 1000_1101_0000_1011; // jump PC=12=R11, R13 = PC+1
   IMEM[19]='b 1010_0000_1000_0010; // store D[R2] = R8  

   
end

always #10 CK = ~CK;

endmodule
