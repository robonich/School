module count4(out,ck);
output
[3:0] out;
input
ck;
reg
[3:0] q;
always @(posedge ck) begin
q <= q+1;
end
assign out = q;
endmodule
