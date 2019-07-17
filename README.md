# Simple DataFlash
A simple DataFlash library for sequential reading and writing. For more information, and the circuit for a simple DataFlash board, see: <http://www.technoblogy.com/show?2JMU>.

The DataFlash library consists of the following seven routines:

#### DataFlash.Setup()

Must be called once to set up the I/O lines to interface with the DataFlash chip. It returns true if the flash chip was found successfully.

#### DataFlash.BeginWrite()

Call this once before writing to the DataFlash. It erases the entire DataFlash chip and sets the address pointer to the start of the chip.

#### DataFlash.WriteByte(uint8_t byte)

Writes a byte of data to the next DataFlash memory location.

#### DataFlash.EndWrite()

Call this once after writing to the DataFlash.

#### DataFlash.BeginRead(unsigned long addr)

Call this once before reading from the DataFlash with the start address for reading.

#### DataFlash.ReadByte()

Reads and returns a byte of data from the next DataFlash memory location.

#### DataFlash.EndRead()

Call this once after reading from the DataFlash.


