module CPU(CK,RST,IA,ID,DA,DD,RW);
   input CK,RST;
   input [15:0] ID;
   output 	RW;
   output [15:0] IA,DA;
   inout [15:0]  DD; 
   reg [15:0] 	 PC, INST, FUA,
		 FUB, LSUA, LSUB, FUC, LSUC, PCC, PCI;
   reg [15:0] 	 RF[0:14];
   reg [1:0] 	 STAGE;
   reg 		 FLAG,RW;
   wire [15:0] 	 ABUS, BBUS, CBUS;
   wire [3:0] 	 OPCODE, OPR1,
		 OPR2, OPR3;
   wire [7:0] 	 IMM;
   wire [15:0] 	 PCn;

   assign PCn = PC + 1;
   assign OPCODE = INST[15:12];
   assign OPR1 = INST[11:8];//C
   assign OPR2 = INST[7:4];//A
   assign OPR3 = INST[3:0];//B
   assign IMM = INST[7:0];
   assign ABUS = RF[OPR2];
   assign BBUS = RF[OPR3];	
   assign DA = LSUB;
   assign IA = PC;
   assign DD =((RW==0)? LSUA : 16'b Z);
   assign CBUS = (OPCODE[3]==0 ? FUC : (OPCODE[3:1]=='b 101 ? LSUC : (OPCODE=='b 1100 ? {8'b 0,IMM} : OPCODE=='b 1000 ? PCC : 'b z)));

   always @(posedge CK) begin
      if( RST == 1 ) begin
	 PC <= 0;
	 STAGE <= 0;
	 RW<=0;
      end else if( STAGE == 0 ) begin
	 INST <= ID;
	 STAGE <= 1;
      end else if( STAGE == 1 ) begin
	 if( (OPCODE[3:0] == 'b 1000) || (OPCODE[3:0] == 'b 1001 && FLAG == 1 ) ) PCI <= BBUS;//jumpとifのBBUSに格納されたプログラムカウンタを予備に入れる。
	 else PCI <= PCn;//命令がjumpとif以外なら、プログラムカウンタの予備を次に進める。

	 if( OPCODE[3] == 0 ) begin//もし演算系の命令であれば
	    FUA <= ABUS; FUB <= BBUS;
	 end else if( OPCODE[2:1] == 'b01) begin//もしload、storeなら
	    LSUA <= ABUS; LSUB <= BBUS;
	 end
	 if( OPCODE[3:1] == 'b 101 ) begin//load, storeなら
	    if( OPCODE[0] == 0 ) begin//store
	       RW <= 0;
	    end else begin//load
	       RW <= 1;
	    end
	 end
	 STAGE <= 2;
      end else if( STAGE == 2 ) begin
	 if( OPCODE[3] == 0 ) begin//演算系なら
	    case(OPCODE[2:0])
	      'b 000: FUC<=FUA+FUB;//加算
	      'b 001: FUC<=FUA-FUB;//減算
	      'b 010: FUC<=FUA>>FUB;//右シフト
	      'b 011: FUC<=FUA<<FUB;//左シフト
	      'b 100: FUC<=FUA|FUB;//論理和
	      'b 101: FUC<=FUA&FUB;//論理積
	      'b 110: FUC<=~FUA;//論理反転 
	      'b 111: FUC<=FUA^FUB;//排他的論理和
	    endcase
	 end else if( OPCODE[3:1] == 'b 101 ) begin//load, storeなら
	    if( OPCODE[0] == 0 ) begin//store
	    end else begin//load
	       LSUC <= DD;//loadされたときの値がDD=LSUAだから、格納された値を読み込む
	    end
	 end else if( OPCODE[3:0] == 'b 1000 ) begin //jumpなら
	    PCC <= PCn;//次に飛ぶ
	 end
	 STAGE <= 3;
      end else if( STAGE == 3 ) begin
	 if( OPCODE[3] == 0 ) begin//演算系なら
	    if( CBUS == 0 ) FLAG <= 1;//
	    else FLAG <= 0;
	 end
	 
	 RF[OPR1] <= CBUS;//loadした結果をIMEM[11:18]に格納される
	 PC <= PCI;//PCが一つ進む
	 STAGE <= 0;
      end
   end

endmodule
