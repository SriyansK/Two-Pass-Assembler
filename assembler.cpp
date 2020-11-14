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
int current_line=0,literal_counter=10000;

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

bool one_operand(string temp)
{
    unordered_map <string,int> ump;
    ump["data"]=-2,ump["ldc"]=0,ump["adc"]=1,ump["ldl"]=2,ump["stl"]=3,ump["ldnl"]=4,ump["stnl"]=5,ump["10"]=6,ump["call"]=13;
    ump["brz"]=15,ump["brlz"]=16,ump["br"]=17,ump["SET"]=-4;
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
    for(char c:temp)
        if(!isdigit(c))
            return false;
    return true;
}

int main()
{
    fstream input_file;
    input_file.open("test3.txt",ios::in);

    vector <string> code; // remove bogus lines and comments and store it in code
    vector <string> errors; //stores the errors
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
            if(s=="")
                continue;
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
    mnemonic["data"]=-2,mnemonic["ldc"]=0,mnemonic["adc"]=1,mnemonic["ldl"]=2,mnemonic["stl"]=3,mnemonic["ldnl"]=4;
    mnemonic["stnl"]=5,mnemonic["add"]=6,mnemonic["sub"]=7,mnemonic["shl"]=8,mnemonic["shr"]=9,mnemonic["adj"]=10;
    mnemonic["a2sp"]=11,mnemonic["s2pa"]=12,mnemonic["call"]=13,mnemonic["return"]=14,mnemonic["brz"]=15,mnemonic["brlz"]=16;
    mnemonic["br"]=17,mnemonic["HALT"]=18,mnemonic["SET"]=-4;

    unordered_map<string,int>  symtab; //symbol table
    vector<pair<string,int>>  litab; // literal table 1st is value and 2nd is address
    for(string s:code) // Here we make symbol table and literal table
    {
        pos=0;
        string temp=sep(s);
        if(islabel(temp)) //If label the 1st string is a label
        {
            if(symtab.find(temp)==symtab.end()) // Here we add labels to symbol table
                symtab[temp]=current_line;
            else if(symtab.find(temp)!=symtab.end() && symtab[temp]!=-1)
            {
                string error="ERROR:Label Repeated at line "; error+=current_line+'0';
                errors.pb(error);
            }
            else if(symtab.find(temp)!=symtab.end() && symtab[temp]==-1)
                symtab[temp]=current_line; // Here adding conditions end
            
            temp="";temp=sep(s); // here we check 2nd string
            if(temp.length()==0)
            {
                current_line++;
                continue;
            }
            else if(ismnemonic(temp,mnemonic)) 
            {
                string mnemo=temp;//store mnemonic in string
                temp="";temp=sep(s); // Here we check if there are more than 1 operand
                string temp1="";temp1=sep(s);
                if(temp1.length()>0) 
                {
                    string error="ERROR:Extra operand at line "; error+=current_line+'0';
                    errors.pb(error);
                    current_line++;
                    continue;
                } // Here checking ends

                if(zero_operand(mnemo))
                {
                    if(temp.length()>0)
                    {
                        string error="ERROR:Extra Operand at line "; error+=current_line+'0';
                        errors.pb(error);
                        current_line++;
                        continue;
                    }
                }
                else if(one_operand(mnemo))
                {
                    if(temp.length()==0)
                    {
                        string error="ERROR:Missing operand at line "; error+=current_line+'0';
                        errors.pb(error);
                    }
                    else if(islabel(temp))
                    {
                        string error="ERROR:Label declared after mnemonic"; error+=current_line+'0';
                        errors.pb(error);
                    }
                    else if(((temp[0]<='z' && temp[0]>='a')||(temp[0]<='Z' && temp[0]>='A')) && symtab.find(temp+":")==symtab.end())//if label is used after mnemonic then insert in map
                        symtab[temp+":"]=-1; //new label found
                    else if(((temp[0]<='z' && temp[0]>='a')||(temp[0]<='Z' && temp[0]>='A') )&& symtab.find(temp+":")!=symtab.end())
                    {
                        current_line++; //label already in table
                        continue;
                    }
                    else if(((temp[0]=='-' || temp[0]=='+') && isdigits(temp.substr(1,temp.length())))||isdigits(temp))//literal table
                        litab.pb({temp,literal_counter++}); // is a number
                    else
                    {
                        string error="ERROR:Wrong Syntax at line "; error+=current_line+'0';
                        errors.pb(error);
                    }
                }
            }
            else
            {
                string error="ERROR:Wrong Syntax at line "; error+=current_line+'0';
                errors.pb(error);
            } // Here checking ends
        }
        else if(ismnemonic(temp,mnemonic)) // If Mnemonic
        {
            string mnemo=temp;//store mnemonic in string
            temp="";temp=sep(s); // Here we check if there are more than 1 operand
            string temp1="";temp1=sep(s);
            if(temp1.length()>0) 
            {
                string error="ERROR:Extra operand at line "; error+=current_line+'0';
                errors.pb(error);
                current_line++;
                continue;
            } // Here checking ends

            if(zero_operand(mnemo))
            {
                if(temp.length()>0)
                {
                    string error="ERROR:Extra Operand at line "; error+=current_line+'0';
                    errors.pb(error);
                    current_line++;
                    continue;
                }
            }
            else if(one_operand(mnemo))
            {
                if(temp.length()==0)
                {
                    string error="ERROR:Missing operand at line "; error+=current_line+'0';
                    errors.pb(error);
                }
                else if(islabel(temp))
                {
                    string error="ERROR:Label declared after mnemonic"; error+=current_line+'0';
                    errors.pb(error);
                }
                else if(((temp[0]<='z' && temp[0]>='a')||(temp[0]<='Z' && temp[0]>='A')) && symtab.find(temp+":")==symtab.end())//if label is used after mnemonic then insert in map
                    symtab[temp+":"]=-1; // new label found
                else if(((temp[0]<='z' && temp[0]>='a')||(temp[0]<='Z' && temp[0]>='A') )&& symtab.find(temp+":")!=symtab.end())
                {
                    current_line++; //label already in table
                    continue;
                }
                else if(((temp[0]=='-' || temp[0]=='+') && isdigits(temp.substr(1,temp.length())))||isdigits(temp))//literal table
                    litab.pb({temp,literal_counter++}); // is a number
                else
                {
                    string error="ERROR:Wrong Syntax at line "; error+=current_line+'0';
                    errors.pb(error);
                }
            }
        }
        else
        {
            string error="ERROR:Wrong Syntax at line "; error+=current_line+'0';
            errors.pb(error);
        }
        current_line++;
    }
    for(auto it:symtab)
        if(it.second==-1)
        {
            string error="ERROR:Undeclared label";errors.pb(error);
        }
}