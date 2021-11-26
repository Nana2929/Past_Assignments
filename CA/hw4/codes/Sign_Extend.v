// reference: https://stackoverflow.com/questions/4176556/how-to-sign-extend-a-number-in-verilog
module Sign_Extend(data_i, data_o);
input [11:0] data_i;
output [31:0] data_o;
assign data_o = {{20{data_i[11]}}, data_i}; // extend the MSB
endmodule


/*
// testbench, instead of writing a Sign_Extend_tb.v
// keep it in a file
module Sign_Ex_tb;
	reg [11:0] A;
	wire [31:0] O;
Sign_Extend uut(A, O);
initial
	begin
		$dumpfile("Sign_Extend.vcd"); //fsdb較大 vcd較快
		$dumpvars(0, Sign_Ex_tb);
		#50 A =12'b000100000000;
        $monitor("A = %b | O = %b", A, O);
		#50 A =12'b111111111110;
        $monitor("A = %b | O = %b", A, O);
		#50 A =12'b111111111110;
        $monitor("A = %b | O = %b", A, O);
		#50 A =12'b111001111110;
        $monitor("A = %b | O = %b", A, O);
		#50 A =12'b011111101110;
        $monitor("A = %b | O = %b", A, O);
        #50 A =12'b011101100110;
        $monitor("A = %b | O = %b", A, O);
		#50 A =12'b111111111111;
        $monitor("A = %b | O = %b", A, O);
		#50 $finish;
	end
endmodule
*/