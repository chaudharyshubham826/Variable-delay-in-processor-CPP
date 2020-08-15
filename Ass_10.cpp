#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <chrono>
#include <random>
using namespace std;

ofstream fileo("out.txt");

string give_register(string a)
{
    string ans = "";
    int i = 0;
    while (a[i] != '(')
    {
        i++;
    }
    // std::fileo << i+2<<'s' << '\n';
    i = i + 1;

    while (a[i] != ')')
    {
        ans = ans + a[i];
        i++;
    }
    return ans;
}

string give_offset(string a)
{ // if 100($t1) then returns 100 as a string
    string ans = "";
    int i = 0;
    while (a[i] != '(')
    {
        ans = ans + a[i];
        i++;
    }
    return ans;
}

int power(int a, int b)
{
    if (b == 0)
    {
        return 1;
    }
    else
    {
        return a * power(a, b - 1);
    }
}

int reg_address(string a)
{
    if (a.compare("$zero") == 0)
    {
        return 0;
    }

    else if (a.compare("$at") == 0)
    {
        return 1;
    }

    else if (a.compare("$v0") == 0)
    {
        return 2;
    }

    else if (a.compare("$v1") == 0)
    {
        return 3;
    }

    else if (a.compare("$sp") == 0)
    {
        return 29;
    }

    else if (a[1] == 'a')
    {
        return 4 + a[2] - '0';
    }

    else if (a[1] == 't' && ((a[2]) != '8' || a[2] != '9'))
    {
        return 8 + a[2] - '0';
    }

    else if (a[1] == 's')
    {
        return 16 + a[2] - '0';
    }

    else if (a[1] == 't')
    {
        return 24 + a[2] - '0';
    }

    else if (a[1] == 'k')
    {
        return 26 + a[2] - '0';
    }

    else if (a == "$gp")
    {
        return 28;
    }

    else if (a == "$fp")
    {
        return 30;
    }

    else if (a == "$ra")
    {
        return 31;
    }

    return -1;
}

void display_register(int ref_file[])
{

    for (int i = 0; i < 32; i++)
    {
        fileo << ref_file[i] << " ";
    }
    fileo << endl;
}

void display_memory(int mem_file[], int size)
{

    for (int i = 0; i < size; i++)
    {
        fileo << mem_file[i] << " ";
    }
    fileo << endl;
}

vector<string> modify(char arr[])
{ ///from a array of characters of the istructions, returns the vetor of wordsofthe input instruction
    vector<string> ans;

    string x = "";
    //fileo<<arr<<endl;
    //fileo << sizeof(arr)<<"-------------"<<endl;

    int i = 0;

    while (i < 100)
    {
        if (arr[i] == '@')
        {
            ans.push_back(x);
            break;
        }

        else if (arr[i] == ' ')
        {
            ans.push_back(x);
            x = "";
            i++;
        }

        else if (arr[i] == ',' && arr[i + 1] == ' ')
        {
            ans.push_back(x);
            x = "";
            i += 2;
        }

        else
        {
            x = x + arr[i];
            i++;
        }
        //fileo<<x<<" x"<<endl;
    }

    return ans;
}

float random_value_generator()
{
    std::random_device rd;
    
    std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );

    std::mt19937 gen(seed);

    
    std::mt19937::result_type n;
    // reject readings that would make n%6 non-uniformly distributed
    while( ( n = gen() ) > std::mt19937::max() -
                                ( std::mt19937::max() - 5 )%6 )
    { /* bad value retrieved so get next one */ }

    n = n %10000007;

    float answer = (float)n/10000006;

    return answer;
}

struct IF_ID_reg
{
    vector<string> instruction;
    // int incremented_pc = -1;
    int read_reg_1 = -1;
    int read_reg_2 = -1;
    int inst_no = -4;
};

struct ID_EX_reg
{
    string id;
    int read_value_1;
    int read_reg_1 = -1;
    int read_reg_2 = -2;

    int read_value_2;
    int write_reg = -3;
    int shift_amount;
    int offset;
    int inst_no = -4;

    // int incremented_pc = -1;
};

struct EX_MEM_reg
{
    string id;
    int result;
    int final_memory_address;
    int write_reg = -3;
    int value_sw;
    int inst_no = -4;
};

struct MEM_WB_reg
{
    string id;
    int data_tobe_written;
    int write_reg = -3;
    int inst_no = -4;
};

int main(int argc, char **argv)
{
    float x = stof(argv[1]);
    int N = stoi(argv[2]);
    float prediction;
    
    

    int register_file[32]; ///register file
    int memory[1024];      //the memory
    int pc = 0;

    for (int i = 0; i < 32; i++)
    { ///////allzero in register file
        register_file[i] = i;
    }

    // register_file[2] = 10;
    // register_file[9] = 10;
    int size = sizeof(memory) / sizeof(*memory);

    for (int i = 0; i < size; i++)
    { ///////allzero in memory
        memory[i] = i;
    }

    // memory[30] = 21;

    vector<vector<string>> instructions; //input instruction

    char inst[100];

    ifstream file("instructions.txt");
    // ofstream fileo("out.txt", ios::app);

    while (!file.eof())
    {

        for (int i = 0; i < 100; i++)
        {
            inst[i] = '@';
        } //////////////////for marking the end of the instruction

        file.getline(inst, 100);

        //fileo<<inst<<endl;
        instructions.push_back(modify(inst));
    }

    file.close();
    // fileo.clear();

    int total_instructions = 0;

    vector<string> nop;
    nop.push_back("nop");
    for (int i = 0; i < 4; i++)
    {
        instructions.push_back(nop);
    }

    int max_pc = instructions.size();

    ////now we have all the instruction in a vector

    //fileo<<instructions.size()<<endl;

    int clock_cycle = 0;

    struct IF_ID_reg if_id_reg;
    struct ID_EX_reg id_ex_reg;
    struct EX_MEM_reg ex_mem_reg;
    struct MEM_WB_reg mem_wb_reg;

    id_ex_reg.id = "nop";
    ex_mem_reg.id = "nop";
    mem_wb_reg.id = "nop";
    if_id_reg.instruction = nop;

    int forward_A;
    int forward_B;
    // bool stall = false;bool valid = true;;

    int no_of_stalls = 0;
    int pc_after_jump = -1;
    int n = 0; //////////////////for saving the no of stalls, so that it can be used while cleaning the instructions vector

    display_register(register_file);
    display_memory(memory, size);
    fileo << "These were the initial values<>" << endl;
    fileo << "-----------------------------------------------------------------------------------------------------------" << endl;
    fileo << endl;

    while (pc >= 0 && pc < max_pc)
    {
        clock_cycle++;

        fileo << "CYCLE " << clock_cycle << " starts:" << endl
              << endl;

        //5th stage starting
        if (mem_wb_reg.id != "nop")
        {

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            string y = mem_wb_reg.id;

            //forwarding unit

            if ((y == "add" || y == "sub" || y == "sll" || y == "srl" || y == "lw") && mem_wb_reg.write_reg != 0)
            {
                if (mem_wb_reg.write_reg == id_ex_reg.read_reg_1)
                {
                    id_ex_reg.read_value_1 = mem_wb_reg.data_tobe_written;
                }

                else if (mem_wb_reg.write_reg == id_ex_reg.read_reg_2)
                {
                    id_ex_reg.read_value_2 = mem_wb_reg.data_tobe_written;
                }
            }

            //forwarding unit

            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (mem_wb_reg.id == "add" || mem_wb_reg.id == "sub" || mem_wb_reg.id == "sll" || mem_wb_reg.id == "srl")
            {
                register_file[mem_wb_reg.write_reg] = mem_wb_reg.data_tobe_written;

                // mem_wb_reg.inst_no = ex_mem_reg.inst_no;
                if (mem_wb_reg.inst_no >= 0)
                {

                    fileo << "Stage 5: " << mem_wb_reg.inst_no << endl;
                }
                // ex_mem_reg.write_reg = -3;
            }

            else if (mem_wb_reg.id == "lw")
            {
                register_file[mem_wb_reg.write_reg] = mem_wb_reg.data_tobe_written;

                if (mem_wb_reg.inst_no >= 0)
                {

                    fileo << "Stage 5: " << mem_wb_reg.inst_no << endl;
                }
                // ex_mem_reg.write_reg = -3;
            }

            else if (mem_wb_reg.id == "jal")
            {
                register_file[mem_wb_reg.write_reg] = mem_wb_reg.data_tobe_written;

                if (mem_wb_reg.inst_no >= 0)
                {

                    fileo << "Stage 5: " << mem_wb_reg.inst_no << endl;
                }
            }

            else
            {
                if (mem_wb_reg.inst_no >= 0)
                {

                    fileo << "Stage 5: " << mem_wb_reg.inst_no << endl;
                }
            }
        }

        else if (mem_wb_reg.id == "nop")
        {
            fileo << "Stage 5: nop" << endl;
        }

        //5th stage ending

        //4th stage starting
        if (ex_mem_reg.id != "nop")
        {
            mem_wb_reg.id = ex_mem_reg.id;

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            string z = ex_mem_reg.id;

            ////forwarding unit
            if ((z == "add" || z == "sub" || z == "sll" || z == "srl" || z == "lw") && ex_mem_reg.write_reg != 0 && ex_mem_reg.write_reg == id_ex_reg.read_reg_1)
            {
                // forward_A = ex_mem_reg.result;
                id_ex_reg.read_value_1 = ex_mem_reg.result;
            }

            else if ((z == "add" || z == "sub" || z == "sll" || z == "srl" || z == "lw") && ex_mem_reg.write_reg != 0 && ex_mem_reg.write_reg == id_ex_reg.read_reg_2)
            {
                // forward_B = ex_mem_reg.result;
                id_ex_reg.read_value_2 = ex_mem_reg.result;
            }

            ////forwarding

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            //stalling for data hazard 2 ----------------------------------------------------------------------------------------------------

            // string y = ex_mem_reg.id;

            // if ((y == "add" || y == "sub" || y == "sll" || y == "srl" || y == "lw") && ex_mem_reg.write_reg != 0)
            // {
            //     if ((ex_mem_reg.write_reg == if_id_reg.read_reg_1 || ex_mem_reg.write_reg == if_id_reg.read_reg_2))
            //     {
            //         instructions.insert(instructions.begin() + pc - 1, nop);
            //         no_of_stalls = 1;
            //         n = 1;
            //         pc_after_jump = pc - 1;
            //         // fileo<<"sjgashjg"<<endl;
            //         total_instructions--;

            //         // fileo << ex_mem_reg.write_reg <<" " <<if_id_reg.read_reg_1 <<" "<< if_id_reg.read_reg_2<<endl;
            //         if_id_reg.instruction = nop;
            //         if_id_reg.read_reg_1 = -1;
            //         if_id_reg.read_reg_2 = -2;
            //         // valid = false;
            //     }
            // }

            //stalling ends -----------------------------------------------------------------------------------------------------------------

            if (mem_wb_reg.id == "add" || mem_wb_reg.id == "sub" || mem_wb_reg.id == "sll" || mem_wb_reg.id == "srl")
            {
                mem_wb_reg.data_tobe_written = ex_mem_reg.result;
                mem_wb_reg.write_reg = ex_mem_reg.write_reg;

                mem_wb_reg.inst_no = ex_mem_reg.inst_no;
                if (ex_mem_reg.inst_no >= 0)
                {

                    fileo << "Stage 4: " << ex_mem_reg.inst_no << endl;
                }
            }

            else if (mem_wb_reg.id == "lw")
            {
                mem_wb_reg.data_tobe_written = memory[ex_mem_reg.final_memory_address];

                /////////////////////////////////////////////////////////////ALUUUUUUUUUUUUUUUUUUUUUUUUU
               

                prediction = random_value_generator();

                ///////////////////////////////////////////////////////////////RANDOM VALUE GENERATION, prediction has the random value now
                // cout<<prediction<<endl;
                if (prediction > x)
                {
                    clock_cycle+=N-1;
                }

                //////////////////////////////////////////////////////////////ALUUUUUUUUUUUUUUUUUUUUUUUUU 
                mem_wb_reg.write_reg = ex_mem_reg.write_reg;

                mem_wb_reg.inst_no = ex_mem_reg.inst_no;
                if (ex_mem_reg.inst_no >= 0)
                {

                    fileo << "Stage 4: " << ex_mem_reg.inst_no << endl;
                }
            }

            else if (mem_wb_reg.id == "sw")
            {
                // cout<<"stststs"<<endl;
                // cout<<ex_mem_reg.final_memory_address<<endl;
                memory[ex_mem_reg.final_memory_address] = ex_mem_reg.value_sw;
                // cout<<"enenen"<<endl;

                mem_wb_reg.inst_no = ex_mem_reg.inst_no;
                if (ex_mem_reg.inst_no >= 0)
                {

                    fileo << "Stage 4: " << ex_mem_reg.inst_no << endl;
                }
            }

            else if (mem_wb_reg.id == "jal")
            {
                mem_wb_reg.data_tobe_written = ex_mem_reg.result;
                mem_wb_reg.inst_no = ex_mem_reg.inst_no;
                mem_wb_reg.write_reg = ex_mem_reg.write_reg;

                if (ex_mem_reg.inst_no >= 0)
                {

                    fileo << "Stage 4: " << ex_mem_reg.inst_no << endl;
                }
            }

            else
            {
                mem_wb_reg.write_reg = -3;

                mem_wb_reg.inst_no = ex_mem_reg.inst_no;
                mem_wb_reg.id = ex_mem_reg.id;
                if (ex_mem_reg.inst_no >= 0)
                {

                    fileo << "Stage 4: " << ex_mem_reg.inst_no << endl;
                }
            }
        }

        else if (ex_mem_reg.id == "nop")
        {
            mem_wb_reg.id = "nop";

            mem_wb_reg.inst_no = -4;
            mem_wb_reg.write_reg = -3;

            fileo << "Stage 4: nop" << endl;
        }

        //4th stage ending

        //3rd stage starting
        if (id_ex_reg.id != "nop")
        {
            ex_mem_reg.id = id_ex_reg.id;

            //for load word data hazard -------------------------------------------------------------------------------------------------------------------------------------------------------

            string y = id_ex_reg.id;

            if ((y == "lw") && id_ex_reg.write_reg != 0)
            {
                if (id_ex_reg.write_reg == if_id_reg.read_reg_1 || id_ex_reg.write_reg == if_id_reg.read_reg_2)
                {
                    instructions.insert(instructions.begin() + pc - 1, nop);
                    total_instructions--;
                    no_of_stalls = 1;
                    n = 1;
                    pc_after_jump = pc - 1;
                    // pc--;
                    if_id_reg.instruction = nop;
                    if_id_reg.read_reg_1 = -1;
                    if_id_reg.read_reg_2 = -2;
                    // valid = false;
                }
            }

            //stalling ends ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

            if (ex_mem_reg.id == "add")
            {
                ex_mem_reg.result = id_ex_reg.read_value_1 + id_ex_reg.read_value_2;
                ex_mem_reg.write_reg = id_ex_reg.write_reg;

                ex_mem_reg.inst_no = id_ex_reg.inst_no;
                if (id_ex_reg.inst_no >= 0)
                {

                    fileo << "Stage 3: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (ex_mem_reg.id == "sub")
            {
                ex_mem_reg.result = id_ex_reg.read_value_1 - id_ex_reg.read_value_2;
                ex_mem_reg.write_reg = id_ex_reg.write_reg;

                ex_mem_reg.inst_no = id_ex_reg.inst_no;
                if (id_ex_reg.inst_no >= 0)
                {

                    fileo << "Stage 3: " << id_ex_reg.inst_no << endl;
                }

                // fileo<<endl;
                // fileo << id_ex_reg.read_value_1<<endl;
                // fileo << id_ex_reg.read_value_2<< endl;
                // fileo << ex_mem_reg.result<<endl;
                // fileo << endl;
            }

            else if (ex_mem_reg.id == "sll")
            {
                ex_mem_reg.result = (id_ex_reg.read_value_1) * power(2, id_ex_reg.shift_amount);
                ex_mem_reg.write_reg = id_ex_reg.write_reg;

                ex_mem_reg.inst_no = id_ex_reg.inst_no;
                if (id_ex_reg.inst_no >= 0)
                {

                    fileo << "Stage 3: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (ex_mem_reg.id == "srl")
            {
                ex_mem_reg.result = (id_ex_reg.read_value_1) / power(2, id_ex_reg.shift_amount);
                ex_mem_reg.write_reg = id_ex_reg.write_reg;

                ex_mem_reg.inst_no = id_ex_reg.inst_no;
                if (id_ex_reg.inst_no >= 0)
                {

                    fileo << "Stage 3: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (ex_mem_reg.id == "lw")
            {
                ex_mem_reg.final_memory_address = id_ex_reg.read_value_1 + id_ex_reg.offset;
                ex_mem_reg.write_reg = id_ex_reg.write_reg;
                
                ex_mem_reg.inst_no = id_ex_reg.inst_no;
                if (id_ex_reg.inst_no >= 0)
                {

                    fileo << "Stage 3: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (ex_mem_reg.id == "sw")

            {
                // cout<<"Inside"<<endl;
                ex_mem_reg.final_memory_address = id_ex_reg.read_value_2 + id_ex_reg.offset;
                // cout << id_ex_reg.read_value_2 << " " << ex_mem_reg.final_memory_address << endl;
                ex_mem_reg.value_sw = id_ex_reg.read_value_1;
                
                ex_mem_reg.inst_no = id_ex_reg.inst_no;
                if (id_ex_reg.inst_no >= 0)
                {

                    fileo << "Stage 3: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (ex_mem_reg.id == "jal")
            {
                ex_mem_reg.result = id_ex_reg.read_value_1;
                ex_mem_reg.inst_no = id_ex_reg.inst_no;
                ex_mem_reg.write_reg = id_ex_reg.write_reg;

                if (id_ex_reg.inst_no >= 0)
                {

                    fileo << "Stage 3: " << id_ex_reg.inst_no << endl;
                }
            }

            else
            {
                ex_mem_reg.write_reg = -3;

                ex_mem_reg.inst_no = id_ex_reg.inst_no;
                ex_mem_reg.id = id_ex_reg.id;
                if (id_ex_reg.inst_no >= 0)
                {

                    fileo << "Stage 3: " << id_ex_reg.inst_no << endl;
                }
            }
        }

        else if (id_ex_reg.id == "nop")
        {
            ex_mem_reg.id = "nop";

            ex_mem_reg.write_reg = -3;
            ex_mem_reg.inst_no = -4;
            fileo << "Stage 3: nop" << endl;
        }
        //3rd stage ending

        ////2nd stage starting

        if (if_id_reg.instruction.at(0) != "nop")
        {

            ////hazard detection unit
            // if (id_ex_reg.id == "lw" && (id_ex_reg.write_reg == if_id_reg.read_reg_1 || id_ex_reg.write_reg == if_id_reg.read_reg_2))
            // {
            //     // lw_hazard = true;

            //     instructions.insert(instructions.begin() + pc, nop);
            //     instructions.insert(instructions.begin() + pc, nop);
            //     no_of_stalls = 2;
            //     n = 2;
            //     pc_after_jump = pc;
            // }

            ///stalling ass8

            //stalling ass8

            id_ex_reg.id = if_id_reg.instruction.at(0);
            if (id_ex_reg.id == "add" || id_ex_reg.id == "sub")
            {
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.read_value_1 = register_file[if_id_reg.read_reg_1];
                id_ex_reg.read_value_2 = register_file[if_id_reg.read_reg_2];
                id_ex_reg.read_reg_2 = if_id_reg.read_reg_2;
                id_ex_reg.write_reg = reg_address(if_id_reg.instruction.at(1));

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }

                // fileo<<endl;
                // fileo << if_id_reg.instruction.at(3) << endl;
            }

            else if (id_ex_reg.id == "sll" || id_ex_reg.id == "srl")
            {
                id_ex_reg.read_value_1 = register_file[if_id_reg.read_reg_1];
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.write_reg = reg_address(if_id_reg.instruction.at(1));
                id_ex_reg.shift_amount = stoi(if_id_reg.instruction.at(3));
                id_ex_reg.read_reg_2 = if_id_reg.read_reg_2;

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (id_ex_reg.id == "lw")
            {
                id_ex_reg.write_reg = reg_address(if_id_reg.instruction.at(1));
                id_ex_reg.read_value_1 = register_file[if_id_reg.read_reg_1];
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.read_reg_2 = if_id_reg.read_reg_2;

                // fileo << "       x" << give_register(if_id_reg.instruction.at(2)) << reg_address(give_register(if_id_reg.instruction.at(2)))<<"x  "<<endl;

                id_ex_reg.offset = stoi(give_offset(if_id_reg.instruction.at(2)));

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (id_ex_reg.id == "sw")
            {
                id_ex_reg.read_value_1 = register_file[if_id_reg.read_reg_1];
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.read_value_2 = register_file[if_id_reg.read_reg_2];
                id_ex_reg.read_reg_2 = if_id_reg.read_reg_2;
                id_ex_reg.write_reg = -3;

                id_ex_reg.offset = stoi(give_offset(if_id_reg.instruction.at(2)));

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (id_ex_reg.id == "beq" || id_ex_reg.id == "bne")
            {
                id_ex_reg.read_value_1 = register_file[if_id_reg.read_reg_1];
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.read_value_2 = register_file[if_id_reg.read_reg_2];
                id_ex_reg.read_reg_2 = if_id_reg.read_reg_2;
                id_ex_reg.write_reg = -3;
                
                id_ex_reg.offset = stoi(if_id_reg.instruction.at(3));

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }

                if ((id_ex_reg.read_value_1 == id_ex_reg.read_value_2) && id_ex_reg.id == "beq")
                {
                    no_of_stalls = 3;
                    n = 2;
                    instructions.insert(instructions.begin() + pc, nop);
                    instructions.insert(instructions.begin() + pc, nop);

                    id_ex_reg.write_reg = -3;

                    pc_after_jump = pc + id_ex_reg.offset;
                }

                else if ((id_ex_reg.read_value_1 != id_ex_reg.read_value_2) && id_ex_reg.id == "bne")
                {
                    no_of_stalls = 3;
                    n = 2;
                    instructions.insert(instructions.begin() + pc, nop);
                    instructions.insert(instructions.begin() + pc, nop);

                    id_ex_reg.write_reg = -3;

                    pc_after_jump = pc + id_ex_reg.offset;
                }
            }

            else if (id_ex_reg.id == "j")
            {
                no_of_stalls = 3;
                n = 2;
                instructions.insert(instructions.begin() + pc, nop);
                instructions.insert(instructions.begin() + pc, nop);
                // instructions.insert(instructions.begin() + pc, nop);

                id_ex_reg.write_reg = -3;
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.read_reg_2 = if_id_reg.read_reg_2;

                pc_after_jump = stoi(if_id_reg.instruction.at(1));

                // stall = true;

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (id_ex_reg.id == "jal")
            {
                // register_file[31] = pc;
                id_ex_reg.write_reg = 31;
                id_ex_reg.read_value_1 = pc;
                no_of_stalls = 3;
                n = 2;
                instructions.insert(instructions.begin() + pc, nop);
                instructions.insert(instructions.begin() + pc, nop);
                // instructions.insert(instructions.begin() + pc, nop);

                // id_ex_reg.write_reg = -3;
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.read_reg_2 = if_id_reg.read_reg_2;

                pc_after_jump = stoi(if_id_reg.instruction.at(1));

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (id_ex_reg.id == "jr")
            {

                no_of_stalls = 3;
                n = 2;
                instructions.insert(instructions.begin() + pc, nop);
                instructions.insert(instructions.begin() + pc, nop);
                // instructions.insert(instructions.begin() + pc, nop);

                id_ex_reg.write_reg = -3;
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.read_reg_2 = if_id_reg.read_reg_2;
                pc_after_jump = register_file[if_id_reg.read_reg_1];

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }
            }

            else if (id_ex_reg.id == "blez")
            {
                id_ex_reg.read_value_1 = register_file[if_id_reg.read_reg_1];
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.read_reg_2 = if_id_reg.read_reg_2;
                id_ex_reg.write_reg = -3;
                

                id_ex_reg.offset = stoi(if_id_reg.instruction.at(2));

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }

                if (id_ex_reg.read_value_1 <= 0)
                {
                    no_of_stalls = 3;
                    n = 2;
                    instructions.insert(instructions.begin() + pc, nop);
                    instructions.insert(instructions.begin() + pc, nop);

                    // id_ex_reg.write_reg = -3;

                    pc_after_jump = pc + id_ex_reg.offset;
                }
            }

            else if (id_ex_reg.id == "bgtz")
            {
                id_ex_reg.read_value_1 = register_file[if_id_reg.read_reg_1];
                id_ex_reg.read_reg_1 = if_id_reg.read_reg_1;
                id_ex_reg.read_reg_2 = -2;
                id_ex_reg.write_reg = -3;
                

                id_ex_reg.offset = stoi(if_id_reg.instruction.at(2));

                id_ex_reg.inst_no = if_id_reg.inst_no;
                if (if_id_reg.inst_no >= 0)
                {

                    fileo << "Stage 2: " << id_ex_reg.inst_no << endl;
                }

                if (id_ex_reg.read_value_1 > 0)
                {
                    no_of_stalls = 3;
                    n = 2;
                    instructions.insert(instructions.begin() + pc, nop);
                    instructions.insert(instructions.begin() + pc, nop);

                    // id_ex_reg.write_reg = -3;

                    pc_after_jump = pc + id_ex_reg.offset;
                }
            }
        }

        else if (if_id_reg.instruction.at(0) == "nop")
        {

            id_ex_reg.id = "nop";

            id_ex_reg.read_reg_1 = -1;
            id_ex_reg.read_reg_2 = -2;
            id_ex_reg.write_reg = -3;
            id_ex_reg.inst_no = -4;
            // id_ex_reg.id = "nop"

            fileo << "Stage 2: nop" << endl;
        }

        ////2nd stage ending

        if (no_of_stalls > 0)
        {
            no_of_stalls -= 1;
            if (no_of_stalls == 0)
            {
                instructions.erase(instructions.begin() + pc - n, instructions.begin() + pc);
                pc = pc_after_jump;
            }
        }

        // if(lw_hazard != true){
        if_id_reg.instruction = instructions.at(pc);
        if_id_reg.inst_no = pc + 1;
        if (if_id_reg.instruction.at(0) != "nop")
        {
            total_instructions++;
        }

        if (if_id_reg.instruction.at(0) == "add" || if_id_reg.instruction.at(0) == "sub")
        {
            if_id_reg.read_reg_1 = reg_address(if_id_reg.instruction.at(2));
            if_id_reg.read_reg_2 = reg_address(if_id_reg.instruction.at(3));

            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
        }

        else if (if_id_reg.instruction.at(0) == "sll" || if_id_reg.instruction.at(0) == "srl")
        {
            if_id_reg.read_reg_1 = reg_address(if_id_reg.instruction.at(2));
            if_id_reg.read_reg_2 = -2;
            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
        }

        else if (if_id_reg.instruction.at(0) == "lw")
        {
            if_id_reg.read_reg_1 = reg_address(give_register(if_id_reg.instruction.at(2)));
            if_id_reg.read_reg_2 = -2;
            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
        }

        else if (if_id_reg.instruction.at(0) == "sw")
        {
            if_id_reg.read_reg_1 = reg_address(if_id_reg.instruction.at(1));
            if_id_reg.read_reg_2 = reg_address(give_register(if_id_reg.instruction.at(2)));
            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
        }

        else if (if_id_reg.instruction.at(0) == "beq" || if_id_reg.instruction.at(0) == "bne")
        {
            if_id_reg.read_reg_1 = reg_address(if_id_reg.instruction.at(1));
            if_id_reg.read_reg_2 = reg_address(if_id_reg.instruction.at(2));
            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
        }

        else if (if_id_reg.instruction.at(0) == "blez")
        {
            if_id_reg.read_reg_1 = reg_address(if_id_reg.instruction.at(1));
            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
            if_id_reg.read_reg_2 = -2;
        }

        else if (if_id_reg.instruction.at(0) == "bgtz")
        {
            if_id_reg.read_reg_1 = reg_address(if_id_reg.instruction.at(1));
            if_id_reg.read_reg_2 = -2;
            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
        }

        else if (if_id_reg.instruction.at(0) == "j")
        {
            if_id_reg.read_reg_1 = -1;
            if_id_reg.read_reg_2 = -2;
            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
        }

        else if (if_id_reg.instruction.at(0) == "jal")
        {
            if_id_reg.read_reg_1 = -1;
            if_id_reg.read_reg_2 = -2;
            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
        }

        else if (if_id_reg.instruction.at(0) == "jr")
        {
            if_id_reg.read_reg_1 = reg_address(if_id_reg.instruction.at(1));
            if_id_reg.read_reg_2 = -2;
            fileo << "Stage 1: " << if_id_reg.inst_no << endl;
        }

        else if (if_id_reg.instruction.at(0) == "nop")
        {
            if_id_reg.read_reg_1 = -1;
            if_id_reg.read_reg_2 = -2;

            if_id_reg.inst_no = -4;

            fileo << "Stage 1: nop" << endl;
        }
        // }

        // else if(if_id_reg.instruction.at(0) == "jr"){
        // if_id_reg.read_reg_1 = reg_address(if_id_reg.instruction.at(1));
        // }

        pc++;

        fileo << "After cycle " << clock_cycle << ":" << endl;
        display_register(register_file);
        display_memory(memory, size);
        fileo << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
        // fileo<<pc<<instructions.size()<<endl;
        fileo << endl;
    }

    fileo << endl;
    fileo<<"x = "<<x<<endl;
    fileo<<"N = "<<N<<endl;
    fileo << "Total clock cycles taken by the program: " << clock_cycle << endl;
    fileo << "Average instructions per cycle for the program: " << ((float)(total_instructions) / (float)(clock_cycle)) << endl;
    fileo << "<-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=->";
    fileo.close();

    return 0;
}