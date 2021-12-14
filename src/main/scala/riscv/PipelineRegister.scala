package riscv

import chisel3._

class PipelineRegister(width: Int = 32) extends Module {
  val io = IO(new Bundle{
    val hold_enable = Input(Bool())
    val in = Input(UInt(width.W))
    val out = Output(UInt(width.W))
  })

  val reg = RegInit(UInt(width.W), 0.U)
  reg := io.in
  io.out := reg
}