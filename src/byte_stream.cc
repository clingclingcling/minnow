#include "byte_stream.hh"
#include<string>
#include<iostream>
using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  if(is_closed() || available_capacity()==0)return ;
  uint64_t to_write;
  to_write=min(available_capacity(),data.size());
  buffer.append(data,0,to_write);
 
  bytes_push+=to_write;
  // Your code here.
}

void Writer::close()
{
   is_ended=true;// Your code here.
}

bool Writer::is_closed() const
{  
  return {is_ended}; // Your code here.
}

uint64_t Writer::available_capacity() const
{
  return capacity_-buffer.size(); // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_push; // Your code here.
}

string_view Reader::peek() const
{
  std::string_view sv=buffer;
  return {sv}; // Your code here.
}

void Reader::pop( uint64_t len )
{ 
  uint64_t pop_len=min(len,buffer.size());
  bytes_pop+=pop_len;
  buffer.erase(0,pop_len);// Your code here.
}

bool Reader::is_finished() const
{ 
  if(is_ended && buffer.size()==0)  return true; 
  return false;// Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return {buffer.size()}; // Your code here.
}

uint64_t Reader::bytes_popped() const
{
  return {bytes_pop}; // Your code here.
}

