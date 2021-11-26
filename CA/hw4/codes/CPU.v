`include "./Registers.v"
`include "./PC.v"
`include "./Instruction_Memory.v"

`include "./Adder.v"
`include "./Sign_Extend.v"
`include "./MUX32.v"
`include "./ALU_Control.v"
`include "./Control.v"
`include "./ALU.v"

/*
Notes:
outputs can be either reg or wire, but an input cannot be a reg.
i: input; o: output
-Sig: signal, eg. RegWriteSig, ALUSrcSig
*/


// top module
module CPU(clk_i, rst_i, start_i);

// top module: Ports
input               clk_i;
input               rst_i;
input              start_i;
wire [31:0] update_addr; //input of PC
wire [31:0] addr; // output of PC
// instantiate module with {module_name} {instance_name}
// and argument specify

assign start_i = 1;

PC PC(
    .clk_i      (clk_i),
    .rst_i      (rst_i),
    .start_i    (start_i), // turn to 1 forever
    .pc_i       (update_addr),
    // updated only when posedge OR reset
    .pc_o       (addr)
);

// save instruction
wire [31:0]        instr;
Instruction_Memory Instruction_Memory(
    .addr_i     (addr),
    .instr_o    (instr)
);

wire [1:0] ALUOp;
wire ALUSrcSig;
wire RegWriteSig;
// instr[6:0]= opcode; goes into control
Control Control(
    .Op_i       (instr[6:0]), //reg in
    .ALUOp_o    (ALUOp), //wire out
    .ALUSrc_o   (ALUSrcSig), // wire out
    .RegWrite_o (RegWriteSig) //wire out
);
// get the 2 values
wire [31:0] data1;
wire [31:0] data2;
wire [31:0] result;
Registers Registers(
    .clk_i      (clk_i),
    .RS1addr_i   (instr[19:15]),
    .RS2addr_i   (instr[24:20]),
    .RDaddr_i   (instr[11:7]),
    .RDdata_i   (result),      // will be written if RegWriteSig is on and data is available
    .RegWrite_i (RegWriteSig),
    .RS1data_o   (data1),
    .RS2data_o   (data2)
);

wire [31:0] ext_immed;
Sign_Extend Sign_Extend(
    .data_i     (instr[31:20]), //if its an immediate op then this value will be immediate field
    .data_o     (ext_immed)
);
// decide whether the op is immediate or not
// A, data1_i: regB
// B, data2_i: immed

wire [31:0] ALUval;
MUX32 MUX_ALUSrc(
    .data1_i    (data2),
    .data2_i    (ext_immed),
    .select_i   (ALUSrcSig),
    .data_o     (ALUval)
);

wire [2:0] ALUCtrlSig; // 3-bit
ALU_Control ALU_Control(
    .funct_i    ({instr[31:25], instr[14:12]}),
    .ALUOp_i    (ALUOp),
    .ALUCtrl_o  (ALUCtrlSig)
);

wire isZero; // not going anywhere though
ALU ALU(
    .data1_i    (data1),
    .data2_i    (ALUval),
    .ALUCtrl_i  (ALUCtrlSig),
    .data_o     (result),
    .Zero_o     (isZero)
);
// update PC value
wire [31:0] const4;
assign const4 = 32'b00000000000000000000000000000100;

Adder Add_PC(
    .data1_in   (addr),
    .data2_in   (const4),
    .data_o     (update_addr)  //update prev_addr; to be replaced in PC at clk posedge/reset negedge
);

endmodule

