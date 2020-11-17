#include <bits/stdc++.h>
#define mp make_pair
#define pb push_back
#define fr first
#define sc second
#define ins insert
#define ii pair<int,int>
#define iii pair<int,pair<int,int>>
#define vi vector <int>
using namespace std;

int pos;
int current_line=0,literal_counter=10000,PC=0;

//convert to decimal 
struct code_listing
{
    string label="";
    string mne="";
    string opera="";
    int pc;
};

bool isdecimal(string value)
{
    if(value[0]=='-' || value[0]=='+')
        value=value.substr(1,value.length());
    if(value[0]=='0' && value.length()==1)
        return true;
    else if(value[0]=='0' && value.length()>1)
        return false;
    for(char c:value) // dec
        if(!isdigit(c))
            return false;
    return true;
}

bool isoctal(string value)
{
    if(value[0]=='0')
    {
        for(char c:value)
            if(c<='7' && c>='0')
                continue;
            else
                return false;
    }
    else
        return false;
    return true;
}

bool islabel(string label)
{
    if(label[label.length()-1]==':' && ((label[0]<='z' && label[0]>='a')||(label[0]<='Z' && label[0]>='A')))
        return true;
    return false;
}

bool ismnemonic(string operand,unordered_map <string,int> mnemonic)
{
    if(mnemonic.find(operand)==mnemonic.end())
        return false;
    return true;
}

bool mnemonic_type_value(string mne)
{
    unordered_map<string,int> value_map;
    value_map["data"]=1,value_map["ldc"]=1,value_map["adc"]=1,value_map["adj"]=1,value_map["SET"]=1;
    if(value_map.find(mne)!=value_map.end())
        return true;
    return false;
}

bool one_operand(string temp)
{
    unordered_map <string,int> ump;
    ump["data"]=19,ump["ldc"]=0,ump["adc"]=1,ump["ldl"]=2,ump["stl"]=3,ump["ldnl"]=4,ump["stnl"]=5,ump["10"]=6,ump["call"]=13;
    ump["brz"]=15,ump["brlz"]=16,ump["br"]=17,ump["SET"]=-4,ump["adj"]=10;
    if(ump.find(temp)!=ump.end())
        return true;
    return false;
}

bool zero_operand(string temp)
{
    unordered_map <string,int> ump;
    ump["add"]=6,ump["sub"]=7,ump["shl"]=8,ump["shr"]=9,ump["a2sp"]=11,ump["sp2a"]=12,ump["return"]=14,ump["HALT"]=18;
    if(ump.find(temp)!=ump.end())
        return true;
    return false;
}

string sep(string s)
{
    for(;pos<s.length();pos++)
        if(isspace(s[pos]))
            continue;
        else
            break;
    string inst="";
    for(;pos<s.length();pos++)
        if(isspace(s[pos]))
            break;
        else
        {
            if(s[pos]==':')
            {
                inst+=s[pos];
                pos++;
                return inst;
            }
            inst+=s[pos];
        }
    return inst;
}

bool isdigits(string temp)
{
    if(temp[0] == '0' && temp[1]== 'x') //hex
    {
        for(int i=2;i<temp.length();i++)
            if(isdigit(temp[i])||(temp[i]<='f' && temp[i]>='a')||(temp[i]<='F' && temp[i]>='A'))
                continue;
            else
                return false;
        return true;
    }
    if(temp[0]=='0' && temp.length()==1) //string temp="0";
        return true;
    if(temp[0]=='0') // octa
    {
        for(int i=1;i<temp.length();i++)
            if(temp[i]<='7' && temp[i]>='0')
                continue;
            else
                return false;
        return true;
    }
    
    for(char c:temp) // dec
        if(!isdigit(c))
            return false;
    return true;
}

int main()
{
    fstream input_file;
    input_file.open("test2.txt",ios::in);

    int line=0;
    vector <string> code; // remove bogus lines and comments and store it in code
    vector <string> errors; //stores the errors
    vector <code_listing> sep_code; //stores each type of string seprately
    if(input_file.is_open())
    {
        string temp;
        while(getline(input_file,temp))
        {
            string s="";
            for(char c:temp)
            {
                if(c==';')
                    break;
                s+=c;
            }
            code.pb(s);
            cout<<temp<<endl;
        }
        input_file.close();
    }
    else
    {
        string error="Empty File";
        errors.pb(error);
        return 0;
    }
    //mnemonic table
    unordered_map <string,int> mnemonic;
    mnemonic["data"]=19,mnemonic["ldc"]=0,mnemonic["adc"]=1,mnemonic["ldl"]=2,mnemonic["stl"]=3,mnemonic["ldnl"]=4;
    mnemonic["stnl"]=5,mnemonic["add"]=6,mnemonic["sub"]=7,mnemonic["shl"]=8,mnemonic["shr"]=9,mnemonic["adj"]=10;
    mnemonic["a2sp"]=11,mnemonic["sp2a"]=12,mnemonic["call"]=13,mnemonic["return"]=14,mnemonic["brz"]=15,mnemonic["brlz"]=16;
    mnemonic["br"]=17,mnemonic["HALT"]=18,mnemonic["SET"]=20;

    unordered_map<string,int>  symtab; //symbol table
    vector<pair<string,int>>  litab; // literal table 1st is value and 2nd is address
    for(string s:code) // Here we make symbol table and literal table
    {
        struct code_listing temp_code;
        pos=0;temp_code.pc=PC;
        string temp=sep(s);
        if(temp=="")
        {
            current_line++;
            continue;
        }
        if(islabel(temp)) //If label the 1st string is a label
        {
            temp_code.label=temp;
            if(symtab.find(temp)==symtab.end()) // Here we add labels to symbol table
                symtab[temp]=PC;
            else if(symtab.find(temp)!=symtab.end() && symtab[temp]!=-1)
            {
                string error="ERROR:Label Repeated at line "; error+=to_string(current_line);
                errors.pb(error);
            }
            else if(symtab.find(temp)!=symtab.end() && symtab[temp]==-1)
                symtab[temp]=PC; // Here adding conditions end
            
            temp="";temp=sep(s); // here we check 2nd string
            if(temp.length()==0)
            {
                sep_code.pb(temp_code);
                current_line++;
                continue;
            }
            else if(ismnemonic(temp,mnemonic)) 
            {
                temp_code.mne=temp;
                string mnemo=temp;//store mnemonic in string
                temp="";temp=sep(s); // Here we check if there are more than 1 operand
                string temp1="";temp1=sep(s);
                if(temp1.length()>0) 
                {
                    string error="ERROR:Extra operand at line "; error+=to_string(current_line);
                    errors.pb(error);
                    current_line++;PC++;
                    continue;
                } // Here checking ends

                if(zero_operand(mnemo))
                {
                    if(temp.length()>0)
                    {
                        string error="ERROR:Extra Operand at line "; error+=to_string(current_line);
                        errors.pb(error);
                        current_line++;PC++;
                        continue;
                    }
                }
                else if(one_operand(mnemo))
                {
                    temp_code.opera=temp;
                    if(temp.length()==0)
                    {
                        string error="ERROR:Missing operand at line "; error+=to_string(current_line);
                        errors.pb(error);
                    }
                    else if(islabel(temp))
                    {
                        string error="ERROR:Label declared after mnemonic"; error+=to_string(current_line);
                        errors.pb(error);
                    }
                    else if(((temp[0]<='z' && temp[0]>='a')||(temp[0]<='Z' && temp[0]>='A')) && symtab.find(temp+":")==symtab.end())//if label is used after mnemonic then insert in map
                        symtab[temp+":"]=-1; //new label found
                    else if(((temp[0]<='z' && temp[0]>='a')||(temp[0]<='Z' && temp[0]>='A') )&& symtab.find(temp+":")!=symtab.end())
                    {
                        sep_code.pb(temp_code);
                        current_line++;PC++; //label already in table
                        continue;
                    }
                    else if(((temp[0]=='-' || temp[0]=='+') && isdigits(temp.substr(1,temp.length())))||isdigits(temp))//literal table
                        litab.pb({temp,literal_counter++}); // is a number
                    else
                    {
                        string error="ERROR:Wrong Syntax at line "; error+=to_string(current_line);
                        errors.pb(error);
                    }
                }
            }
            else
            {
                string error="ERROR:Wrong Syntax at line "; error+=to_string(current_line);
                errors.pb(error);
            } // Here checking ends
        }
        else if(ismnemonic(temp,mnemonic)) // If Mnemonic
        {
            temp_code.mne=temp;
            string mnemo=temp;//store mnemonic in string
            temp="";temp=sep(s); // Here we check if there are more than 1 operand
            string temp1="";temp1=sep(s);
            if(temp1.length()>0) 
            {
                string error="ERROR:Extra operand at line "; error+=to_string(current_line);
                errors.pb(error);
                current_line++;PC++;
                continue;
            } // Here checking ends

            if(zero_operand(mnemo))
            {
                if(temp.length()>0)
                {
                    string error="ERROR:Extra Operand at line "; error+=to_string(current_line);
                    errors.pb(error);
                    current_line++;PC++;
                    continue;
                }
            }
            else if(one_operand(mnemo))
            {
                temp_code.opera=temp;
                if(temp.length()==0)
                {
                    string error="ERROR:Missing operand at line "; error+=to_string(current_line);
                    errors.pb(error);
                }
                else if(islabel(temp))
                {
                    string error="ERROR:Label declared after mnemonic"; error+=to_string(current_line);
                    errors.pb(error);
                }
                else if(((temp[0]<='z' && temp[0]>='a')||(temp[0]<='Z' && temp[0]>='A')) && symtab.find(temp+":")==symtab.end())//if label is used after mnemonic then insert in map
                    symtab[temp+":"]=-1; // new label found
                else if(((temp[0]<='z' && temp[0]>='a')||(temp[0]<='Z' && temp[0]>='A') )&& symtab.find(temp+":")!=symtab.end())
                {
                    sep_code.pb(temp_code);
                    current_line++;PC++; //label already in table
                    continue;
                }
                else if(((temp[0]=='-' || temp[0]=='+') && isdigits(temp.substr(1,temp.length())))||isdigits(temp))//literal table
                    litab.pb({temp,literal_counter++}); // is a number
                else
                {
                    string error="ERROR:Wrong Syntax at line "; error+=to_string(current_line);
                    errors.pb(error);
                }
            }
        }
        else
        {
            string error="ERROR:Wrong Syntax at line "; error+=to_string(current_line);
            errors.pb(error);
        }                                                                                                                              
        sep_code.pb(temp_code);
        current_line++;PC++;
    }
    for(auto it:symtab)
        if(it.second==-1)
        {
            string error="ERROR:Undeclared label "+it.first.substr(0,it.first.length()-1);errors.pb(error);
        }
    unordered_map<string,int> data;
    for(auto it:sep_code)
        if(it.mne=="data" && it.label!="")
            data[it.label]=1;
    //pass 1 complete
    if(errors.size()>0)
    {
        //Log file
        ofstream log_file;
        log_file.open("log.txt");

        int error_no=1;
        for(auto it:errors)
        {
            log_file<<error_no<<". "<<it<<endl;
            error_no++;
        }
        log_file.close();
        return 0;
    }

    //pass 2 start

    //listing file
    ofstream listing_file;
    listing_file.open("listing_file.txt");
    vector <string> machine_code;
    for(auto it:sep_code)
    {
        stringstream ss;// adding PC to listing file
        ss<<hex<<it.pc;
        string res(ss.str());
        int len=res.length();
        for(int i=0;i<8-len;i++)
            listing_file<<"0";
        listing_file<<res<<" ";
        if(it.mne!="") //check if mnemonic
        {
            stringstream ss1;
            ss1<<hex<<mnemonic[it.mne];
            string res1(ss1.str());
            if(zero_operand(it.mne)) // if zero type operand
            {
                for(int i=0;i<8-res1.length();i++)
                    listing_file<<"0";
                listing_file<<res1<<" ";
            }
            else // if one type operand
            {
                if(isdigits(it.opera) || isdigits(it.opera.substr(1,it.opera.length()))) // is a number
                {
                    //cout<<it.opera<<endl;
                    if(isdecimal(it.opera))
                    {
                        int decimal_digit=stoi(it.opera);
                        stringstream ss2;
                        ss2<<hex<<decimal_digit;
                        string res2(ss2.str());
                        if(decimal_digit<0)
                            res2=res2.substr(2,res2.length());
                        for(int i=0;i<6-res2.length();i++)
                            listing_file<<"0";
                        listing_file<<res2;
                        for(int i=0;i<2-res1.length();i++)
                        listing_file<<"0";
                        listing_file<<res1<<" ";
                    }
                    else if(isoctal(it.opera))
                    {
                        int decimal_value;
                        stringstream ss2;
                        ss2<<it.opera;
                        ss2>>oct>>decimal_value;

                        stringstream ss3;
                        ss3<<hex<<decimal_value;
                        string res2(ss3.str());
                        for(int i=0;i<6-res2.length();i++)
                            listing_file<<"0";
                        listing_file<<res2;
                        for(int i=0;i<2-res1.length();i++)
                        listing_file<<"0";
                        listing_file<<res1<<" ";
                    }
                    else
                    {
                        string hexa=it.opera.substr(2,it.opera.length());
                        for(int i=0;i<6-hexa.length();i++)
                            listing_file<<"0";
                        listing_file<<hexa;
                        for(int i=0;i<2-res1.length();i++)
                            listing_file<<"0";
                        listing_file<<res1<<" ";
                    }
                }
                else if(data.find(it.opera+":")!=data.end()) // is a variable
                {
                    stringstream ss2;
                    ss2<<hex<<symtab[it.opera+":"];
                    string res2(ss2.str());
                    for(int i=0;i<6-res2.length();i++)
                        listing_file<<"0";
                    listing_file<<res2;
                    for(int i=0;i<2-res1.length();i++)
                        listing_file<<"0";
                    listing_file<<res1<<" ";
                }
                else // is a label
                {
                    if(mnemonic_type_value(it.mne))
                    {
                        stringstream ss2;
                        ss2<<hex<<(symtab[it.opera+":"]);
                        string res2(ss2.str());
                        for(int i=0;i<6-res2.length();i++)
                            listing_file<<"0";
                        listing_file<<res2;
                        for(int i=0;i<2-res1.length();i++)
                            listing_file<<"0";
                        listing_file<<res1<<" ";
                    }
                    else
                    {
                        stringstream ss2;
                        ss2<<hex<<(symtab[it.opera+":"]-it.pc-1);
                        string res2(ss2.str());
                        if(symtab[it.opera+":"]-it.pc-1<0)
                            res2=res2.substr(2,res2.length());
                        for(int i=0;i<6-res2.length();i++)
                            listing_file<<"0";
                        listing_file<<res2;
                        for(int i=0;i<2-res1.length();i++)
                            listing_file<<"0";
                        listing_file<<res1<<" ";
                    }
                }
            }
        }
        else // if no mnemonic then print spaces
        {
            for(int i=0;i<8;i++)
                listing_file<<" ";
        }

        if(it.label!="")
            listing_file<<it.label<<" ";
        if(it.mne!="")
            listing_file<<it.mne<<" ";
        if(it.opera!="")
            listing_file<<it.opera<<" ";
        listing_file<<endl;
    }
    listing_file.close();

}