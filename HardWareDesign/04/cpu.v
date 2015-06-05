module CPU(CK, RST, IA, ID, DA, DD, RW);
   input CK, RST;
   input [15:0] ID;
   output 	RW;
   output [15:0] IA, DA;
   inout [15:0]  DD;
   wire [3:0] 	 OPCODE, OPR1, OPR2, OPR3;
   wire [7:0] 	 IMM;
   reg [15:0] 	 INST;
   wire [15:0] 	 BUSA, BUSB, BUSC;
   reg [15:0] 	 LSUA, LSUB, LSUC;
   reg [15:0] 	 FUA, FUB, FUC;
   wire [15:0] 	 DD;
   wire [15:0] 	 IA, DA;
   reg [15:0] 	 RF [0:14];
   reg 		 FLAG;
   reg [15:0] 	 PCI, PC, PCC;
   reg 		 RW;
   reg [1:0] 	 STAGE;

   assign IA = PC;
   assign OPCODE = INST[15:12];
   assign OPR1 = INST[11:8];
   assign OPR2 = INST[7:4];
   assign OPR3 = INST[3:0];
   assign IMM = INST[7:0];
   assign BUSA = RF[OPR2];//OPR2
   assign BUSB = RF[OPR3];//OPR3
   assign DD = (RW == 0 ? LSUA : 'b Z);
   assign DA = LSUB;
   assign BUSC = (OPCODE[3] == 0 ? FUC : (OPCODE[3:1] == 'b101 ? LSUC : (OPCODE == 'b1100 ? {8'b 0, IMM} : (OPCODE == 'b1000 ? PCC : 'b Z))));
   
   
   always @(posedge CK) begin
      if(RST == 1) begin
	 STAGE <= 0;
	 PC <= 0;
	 RW <= 0;
	 
      end else begin
	 
	 if(STAGE == 0) begin
	    INST <= ID;
	    
	    STAGE <= 1;
	    
	 end else if(STAGE == 1) begin
	    if(OPCODE[3] == 0) begin
	       FUA <= BUSA;
	       FUB <= BUSB;
	    end
	    if(OPCODE[2:1] == 'b01) begin
	       LSUA <= BUSA;
	       LSUB <= BUSB;
	    end
	    if((OPCODE[3:0] == 'b1000) || (OPCODE[3:0] == 'b1001 && FLAG == 1)) begin
	       PCI <= BUSB;
	    end else begin
	       PC <= INST[15];
	       PCI <= PC + 1;
	    end
	    STAGE <= 2;
	    
	 end else if(STAGE == 2) begin // if (STAGE == 1)
	    
	    if(OPCODE[3] == 0) begin
	       case (OPCODE[2:0])
		 'b000: FUC <= FUA + FUB;
		 'b001: FUC <= FUA - FUB;
		 'b010: FUC <= FUA >> FUB;
		 'b011: FUC <= FUA << FUB;
		 'b100: FUC <= FUA | FUB;
		 'b101: FUC <= FUA & FUB;
		 'b110: FUC <= ~FUA;
		 'b111: FUC <= FUA ^ FUB;
	       endcase // case (OPCODE[2:0])
	    end else if(OPCODE[3:1] == 'b101) begin
	       if(OPCODE[0] == 0) begin
		  RW <= 0;
		  
	       end else begin
		  RW <= 1;
		  LSUC <= DD;
	       end
	    end else if(OPCODE[3:0] == 'b1001) begin
	       if(flag) begin
		  PC <= FUB;
	       end
	    end else if(OPCODE[3:0] == 'b1000) begin
	       PCC <= PC;
	    end
	    STAGE <= 3;
	    
	 end else if(STAGE == 3) begin // if (STAGE == 2)
	    RF[OPR1] <= BUSC;
	    PC <= PCI;
	    
	    STAGE <= 0;
	 end
	 
      end // else: !if(RST == 1)
   end // always @ (posedge CK)
endmodule // CPU
