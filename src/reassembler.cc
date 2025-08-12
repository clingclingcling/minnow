#include "reassembler.hh"
#include "debug.hh"

using namespace std;
Reassembler::Reassembler(ByteStream&& output)
    : output_(std::move(output)),
      inter_store(/*用后面计算出的_capacity先占位，后面再assign*/),/*构造函数需与声明顺序一致*/
      is_filled(/*同上*/),
      head_index(0),
      last_index(0),
      _capacity(output_.writer().available_capacity()),
      eof(false)
{
    /*后赋值*/
    _capacity = output_.writer().available_capacity();
    inter_store.assign(_capacity, '\0');
    is_filled.assign(_capacity, false);
}


void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{  
   
   string to_write={};
   
    uint64_t n=0;
    if (is_last_substring) {
        eof = true;
        last_index = first_index + data.size();
    }

    if(data.empty() && is_last_substring){
        output_.writer().close();
         return ;
    }
   if(first_index>=head_index+_capacity || first_index+data.size()<=head_index)return;

   for(uint64_t i=0;i<data.size();i++){
    if(is_filled[first_index-head_index+i] || first_index+i<head_index )continue;
    if(first_index-head_index+i>_capacity)break;
    else{
        inter_store[first_index-head_index+i]=data[i];
        is_filled[first_index-head_index+i]=true;
    }
   }
   while(n < _capacity && is_filled[n]){
    to_write.push_back(inter_store[n]);
    n++;
   }
   if(n!=0){
    head_index+=n;
    inter_store.erase(0,n);
    inter_store.insert(inter_store.end(),n,'\0');
    is_filled.erase(is_filled.begin(),is_filled.begin()+n);
    is_filled.insert(is_filled.end(),n,false);
    output_.writer().push(to_write);
   }
   if(eof && head_index>=last_index){
    output_.writer().close();
   }

}

// How many bytes are stored in the Reassembler itself?
// This function is for testing only; don't add extra state to support it.
uint64_t Reassembler::count_bytes_pending() const
{
 uint64_t ans=0;
 for(uint64_t i=0;i<_capacity;i++){
     if(is_filled[i])ans++;
 }
 return ans;
}
