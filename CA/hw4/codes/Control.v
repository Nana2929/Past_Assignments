
module Control(Op_i, ALUOp_o, ALUSrc_o, RegWrite_o);
input [6:0] Op_i; // 7-bit opcode in instruction
output reg [1:0] ALUOp_o; // 2-bit // load or store: 00; branch: 01; R-type: 10; others I'll use 11
output reg ALUSrc_o; // 1-bit, select = 1-> output immed(B), otherwise output regB(A)
output reg RegWrite_o; //1-bit, 有rd(dest reg) = 1

always @(*) begin
    case (Op_i)
        7'b0110011:begin
            // R-type instruction
            ALUSrc_o = 1'b0;
            ALUOp_o = 2'b10;
            RegWrite_o = 1'b1;
            end
        7'b0010011:begin
            // I-type
            ALUSrc_o = 1'b1;
            ALUOp_o = 2'b11;
            RegWrite_o = 1'b1;
        end
        default:begin
            // default R-type
            ALUSrc_o = 1'b0;
            ALUOp_o = 2'b10;
            RegWrite_o = 1'b1;
            end
    endcase
end
endmodule

/*
// test bench
module Control_tb;
reg [6:0] A;
// output signal
wire [1:0] Aop;
wire Asc;
wire Rw;
Control uut(A, Aop, Asc, Rw); //by-order passing arguments
initial
	begin
		$dumpfile("Control_tb.vcd"); //fsdb較大 vcd較快
		$dumpvars(0, Control_tb);
		#50 A =7'b0110011;
        #50 A =7'b0110011;
        #50 A =7'b0110011;
        #50 A =7'b0010011; //
        #50 A =7'b0110011;
        #50 A =7'b0010011; //
		#50 $finish;
	end
endmodule
*/