module FullAdderFunction(x,y,cin,cout,s);
   output cout;
   output s;
   input  x,y,cin;
   assign cout = (x&y)|(x&cin)|(y&cin);
   assign s = x^y^cin;
endmodule // FullAdderFunction
