/*                                  
                            Global Documentation

Program Name: Stimulation Programming for Booth's Multiplier
Name: Harsh Mohan Sason
Lab no: 5

**Program Description**

- In this program we do Stimulation programming in which we build a simulator 
for Booth's 2 compilement number multiplier. We do that by creating 3 functions.

    1. ALU_1bit which has 4 parameters. The first one is the first two para-
meters named a and b are integer type variables and they are the two input bits
for the 1 bit ALU. C_in is the carry in and OP is the operand type which holds
addition and subtraction operations.
 
    2. ALU_16bit which has 3 parameters as. The first two are of bitset 
types which only take inputs in 1 and 0's. The third one is of string type and 
takes OP value as either add or sub. We call the ALU_1bit 16 times and run it 
through a loop to get our operations done for addition and subtraction for 16
bit numbers.
 
    3. Booth_Mult has two parameters MD and MQ. MD is the multiplicand and MQ is 
the multiplier. both are of types bitset i.e they only take input values as 1's
and 0's. We also have a cycle_counter named contructor which is also of bitset 
type and is initialized to 15 i.e 1111.
 
    4. int main() which is the driver function which takes input values of MD 
& MQ and calls the function Booth_Mult returning the product and the output.

**Methodologies**:

- We used the help of bitset library to make sure we get the desired output 
in terms of 0's and 1's.
- We called the function ALU_1bit 16 times and used the principle of ripple 
carry where the carry_in values is used as the carry_out value of the next and 
vice-versa for the next ALU.
- We only called the ALU_16bit function when there is a case of XOR i.e one of 
the input needs to be true.

*/


#include <iostream>		// input/output
#include <bitset>		// bitset library to deal with only 0's and 1's
using namespace std;


int N;				    // Global variable for the number of bits

unsigned short accumulator = 0;
unsigned short s = 0;
bitset < 16 > AC
{
accumulator};			// contructor named AC 

bitset < 16 > result
{
s};				        // constructor named result to store the 16 bit result


int
ALU_1bit (int a, int b, int C_in, string OP)	//function to implement subtraction and addition
{
  int C_out = 0;
  if (OP == "ADDITION")
    {
      if (N < 16)		//to make sure to store 16 bits only
	{
	  result[N] = a ^ b ^ C_in;	//to get the resulting sum
	  N++;
	}

      C_out = (a * b) + C_in * (a ^ b);	// to get the carryout value
    }
  else if (OP == "SUBTRACTION")
    {
      if (b == 0)
	{
	  b = 1;
	}

      else
	{
	  b = 0;
	}
      if (N < 16)                //to make sure to store 16 bits only
	{
	  result[N] = a ^ b ^ C_in;	// to get the resulting sub value
	  N++;
	}

      C_out = (a * b) + C_in * (a ^ b);	// to get the resulting carry out


    }
  return C_out;
}

int
ALU_16bit (bitset < 16 > M, bitset < 16 > N, string OP)	//16 bit ALU function which uses 1 bit ALU 16 times to do all the 1 bit ALU function on a 16 bit Number and stores the resulting output in the result 
{
  int Carry_in_out = 0;
  N = 0;
  result = 0;			//clear resulting bits to all 0.

  if (OP == "SUBTRACTION")	//to make sure if the operation is Sub, the resulting carry value is 1
    {
      if (M[0] == 0 && N[0] == 1)
	{
	  Carry_in_out = 1;
	}
    }
  for (int i = 0; i < 16; i++)
    {
      Carry_in_out = ALU_1bit (M[i], N[i], Carry_in_out, OP);	//we use ripple carry to get the resulting output
    }

  AC = result;
  return Carry_in_out;
}

int
Booth_Mult (bitset < 16 > MD, bitset < 16 > MQ)
{

  unsigned short MQ0 = 0;
  unsigned short counter = 15;
  bitset < 1 > MQ_1
  {
  MQ0};
  bitset < 4 > cycle_counter
  {
  counter};

  cout << endl << endl;
  cout <<
    " Cycle-Counter	      MD			AC		       MQ		MQ_1"
    << endl;
  cout <<
    " -------------------------------------------------------------------------------------------"
    << endl;
  cout << " " << cycle_counter << "		" << MD << "	" << AC <<
    "	" << MQ << "	  " << MQ_1 << endl;
  for (int k = 0; k < 16; k++)
    {
      if (MQ[0] == 0 && MQ_1[0] == 0)
	{
	  AC = AC;
	  cout << " " << cycle_counter << "		" << MD << "	" <<
	    AC << "	" << MQ << "	  " << MQ_1 << endl;

	  MQ_1[0] = MQ[0];	//airhtmetic shift right (SAR) for MQ_1

	  MQ = MQ >> 1;		//airthmetic shift right (SAR) for MQ
	  MQ[15] = AC[0];

	  AC = AC >> 1;		//airthmetic shift right (SAR) for AC
	  AC[15] = AC[14];


	}

      else if (MQ[0] == 0 && MQ_1[0] == 1)
	{
	  ALU_16bit (AC, MD, "ADDITION");	//AC + MD

	  cout << " " << cycle_counter << "		" << MD << "	" <<
	    AC << "	" << MQ << "	  " << MQ_1 << endl;

	  MQ_1[0] = MQ[0];	//airhtmetic shift right (SAR) for MQ_1

	  MQ = MQ >> 1;		//airthmetic shift right (SAR) for MQ
	  MQ[15] = AC[0];

	  AC = result;
	  AC = AC >> 1;		//airthmetic shift right (SAR) for AC
	  AC[15] = AC[14];
	}
      else if (MQ[0] == 1 && MQ_1[0] == 0)
	{
	  ALU_16bit (AC, MD, "SUBTRACTION");	//AC - MD
	  cout << " " << cycle_counter << "		" << MD << "	" <<
	    AC << "	" << MQ << "	  " << MQ_1 << endl;

	  MQ_1[0] = MQ[0];
	  MQ = MQ >> 1;
	  MQ[15] = AC[0];

	  AC = AC >> 1;
	  AC[15] = AC[14];


	}
      else if (MQ[0] == 1 && MQ_1[0] == 1)
	{
	  AC = AC;
	  cout << " " << cycle_counter << "		" << MD << "	" <<
	    AC << "	" << MQ << "	  " << MQ_1 << endl;

	  MQ_1[0] = MQ[0];
	  MQ = MQ >> 1;
	  MQ[15] = AC[0];

	  AC = AC >> 1;
	  AC[15] = AC[14];

	}

      cout << " " << cycle_counter << "		" << MD << "	" << AC <<
	"	" << MQ << "	  " << MQ_1 << endl << endl;
      counter--;		//decrement cycle counter
      cycle_counter = counter;
    }
  cout << '\n' << '\n';

  return 0;

}


int
main ()
{
  int product;

  bitset < 16 > MD ("1111111110101000");
  bitset < 16 > MQ ("1111111110110100");

  // Input1
  // MD = 0000 0010 0001 1001
  // MQ = 1111 1111 1110 0110 

  // Input2
  // MD = 1111 1111 1110 0110
  // MQ = 0000 0010 0001 1001

  // Input3
  // MD = 1111 1111 1010 1000
  // MQ = 1111 1111 1011 0100

  cout << endl;
  cout << " MD: " << MD << endl << endl;
  cout << " MQ: " << MQ << endl;

  product = Booth_Mult (MD, MQ);	//call the Booth_Multiplier


  cout << product;



  return product;
}
