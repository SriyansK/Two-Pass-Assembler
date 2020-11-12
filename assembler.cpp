#include <bits/stdc++.h>
#define ll long long int
#define mp make_pair
#define pb push_back
#define pp pop_back
#define fr first
#define sc second
#define ins insert
#define lb lower_bound
#define ub upper_bound
#define ii pair<int,int>
#define iii pair<int,pair<int,int>>
#define vi vector <int>
#define ascii_a 97
#define ascii_z 48
#define ascii_A 65
#define pi 3.14159265358979323846
using namespace std;
 
const ll N =1e5+10;
const int MOD=1e9+7;
const long long IINF=1e18;

int pos;
int current_line=0,literal_counter=10000;


/*
    correct literals according to mnemonic and perfect the location counter.
*/

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

void printlabel(unordered_map <string,int> symtab)
{
    for(auto it:symtab)
        cout<<it.first<<" "<<it.second<<endl;
}

void printlit(vector <pair<string,int>> litab)
{
    for(auto it:litab)
        cout<<it.first<<" "<<it.second<<endl;
}

void printerror(vector <string> errors)
{
    for(auto s:errors)
        cout<<s<<endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
	cin.tie(NULL);

    //taking input from file
    fstream input_file;
    input_file.open("test4.txt",ios::in);

    //removing comments and bogus lines
    vector <string> code; // stores the separated input
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
    for(string s:code)
    {
        pos=0;
        string temp=sep(s);
        if(islabel(temp)) //If label the 1st string is a label
        {
            //process label
            if(symtab.find(temp)==symtab.end())
                symtab[temp]=current_line;
            else if(symtab.find(temp)!=symtab.end() && symtab[temp]!=-1)
            {
                string error="ERROR:Label Repeated at line "; error+=current_line+'0';
                errors.pb(error);
            }
            else if(symtab.find(temp)!=symtab.end() && symtab[temp]==-1)
                symtab[temp]=current_line;
            // process label ends
            temp="";temp=sep(s); // find the 2nd string
            if(temp.length()==0) // if string is empty then continue;
            {
                current_line++;
                continue;
            }
            if(ismnemonic(temp,mnemonic)) //check if mnemonic
            {
                temp="";temp=sep(s);//3rd string
                string temp1="";temp1=sep(s);//if 4th string
                if(temp1.length()>0)
                {
                    string error="ERROR:Extra variable at line "; error+=current_line+'0';
                    errors.pb(error);
                    current_line++;
                    continue;
                }
                if(temp.length()==0)//if empty
                {
                    current_line++;
                    continue;
                }
                if(islabel(temp))// If label is declared after mnemonic 
                {
                    string error="ERROR:Wrong Syntax at line"; error+=current_line+'0';
                    errors.pb(error);
                }
                if(!isdigit(temp[0]) && temp[0]!='-' && temp[0]!='+' && symtab.find(temp+":")==symtab.end())//if label is used after mnemonic then insert in map
                    symtab[temp+":"]=-1;
                else if(!isdigit(temp[0]) && temp[0]!='-' && temp[0]!='+' && symtab.find(temp+":")!=symtab.end())
                    continue;
                else if((temp[0]=='-' || temp[0]=='+') && isdigits(temp.substr(1,temp.length())))//literal table
                    litab.pb({temp,literal_counter++});
                else if(isdigits(temp))
                    litab.pb({temp,literal_counter++});
                else
                {
                    string error="ERROR:Wrong Syntax at line "; error+=current_line+'0';
                    errors.pb(error);
                }
            }
            else
            {
                string error="ERROR:Wrong Syntax at line "; error+=current_line+'0';
                errors.pb(error);
            }
        }
        else if(ismnemonic(temp,mnemonic)) // If Mnemonic
        {
            // find after mnemonic
            temp="";temp=sep(s);
            string temp1="";temp1=sep(s);//if 4th string
            if(temp1.length()>0)
            {
                string error="ERROR:Extra variable at line "; error+=current_line+'0';
                errors.pb(error);
                current_line++;
                continue;
            }
            if(temp.length()==0)
            {
                current_line++;
                continue;
            }
            if(islabel(temp))// If label is declared after mnemonic 
            {
                string error="ERROR:Wrong Syntax at line "; error+=current_line+'0';
                errors.pb(error);
            }
            if(!isdigit(temp[0]) && temp[0]!='-' && temp[0]!='+' && symtab.find(temp+":")==symtab.end())//if label is used after mnemonic then insert in map
                symtab[temp+":"]=-1;
            else if(!isdigit(temp[0]) && temp[0]!='-' && temp[0]!='+' && symtab.find(temp+":")!=symtab.end())
                    continue;
            else if((temp[0]=='-' || temp[0]=='+') && isdigits(temp.substr(1,temp.length())))//literal table
                litab.pb({temp,literal_counter++});
            else if(isdigits(temp))
                litab.pb({temp,literal_counter++});
            else
            {
                string error="ERROR:Wrong Syntax at line "; error+=current_line+'0';
                errors.pb(error);
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
            string error="ERROR:Undeclared label";
            errors.pb(error);
        }
    /*printlabel(symtab);
    printlit(litab);
    printerror(errors);*/
    if(1)
        cout<<"YES"<<endl;
}