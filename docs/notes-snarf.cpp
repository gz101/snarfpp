#include<iostream>
#include<algorithm>
#include<cmath>
#include <random>
#include <fstream>
#include <chrono>
#include <set>
#include <ctime>
#include <cstring>
using namespace std;
using namespace std::chrono; 

// #include "snarf_model.cpp"
// #include "snarf_bitset.cpp"

//SNARF implementation which uses Golomb Coding(GCS)
template <class T>
struct snarf_gcs
{
  //Snarf model
  snarf_model<T> rmi;

  //snarf bit array
  vector< snarf_bitset > bb_bitset_vec;

  //Parameters used in snarf
  uint64_t N,P,block_size,bit_size,total_blocks;
  uint64_t gcs_size;

  //Stores the number of keys in each bit array block
  vector<int> vec_num_keys;
  

  //Used for Inference
  uint64_t delta_query_index=0;
  uint64_t delta_query_remainder=0;
  uint64_t delta_query_val_until_now=0;
  uint64_t delta_query_data_size=0;
  uint64_t delta_query_offset_1=0;
  uint64_t delta_query_offset_2=0;
  uint64_t delta_query_zero_count=0;
  uint64_t delta_query_one_count=0;
  double query_cdf1,query_cdf2=0.0;

  //Name of snarf instance
  char name_curr;

  snarf_gcs<T>(){

    N=0;
    P=0;
    block_size=0;
    bit_size=0;
    total_blocks=0;
    gcs_size=0;
    name_curr=' ';
  }

 


  //Get the bit locations of the bits that need to be set to 1
  void get_locations(vector<T> &keys,vector<uint64_t> &temp_locations)
  {
    double cdf;
    uint64_t temp_loc;

    sort(keys.begin(),keys.end());

    //Iterate over the key array to find the bit location corresponding to each key
    uint64_t past_loc=0;
    for(int i=0;i<keys.size();i++)
    { 
      cdf=rmi.infer(keys[i]);   // use model to predict cdf of key
      
      temp_loc=floor(cdf*N*P);  // get location in bit array based on cdf
      temp_loc=max((uint64_t)0,temp_loc);   // if too low set it to index 0 in bit array
      temp_loc=min(N*P-1,temp_loc);         // if too high set it to last index in bit array
      temp_locations.push_back(temp_loc);   // add key location
      past_loc=temp_loc;
    }


    sort(temp_locations.begin(),temp_locations.end());  // assuming keys are not sorted

    return ;
  }

  //Create a new bit block(var bb_temp) for a batch of values(curr_batch).
  void create_new_gcs_block(vector<uint64_t> &curr_batch, snarf_bitset &bb_temp)
  {
    // resize the bit array for this new batch
    bb_temp.init((bit_size+1)*curr_batch.size()+block_size);

    int offset_bits=0;

    //Write the binary code in the bit array continuously
    for(int i=0;i<curr_batch.size();i++)
    {
      bb_temp.bitset_write_bits(offset_bits,curr_batch[i]%P,bit_size);  // binary value is the remainder
      offset_bits+=bit_size;
    }

    //Write the unary code in the bit array continuously after binary code
    int delta_zero_count=0,delta_one_count=0;
    for(int i=0;i<curr_batch.size();i++)
    {

      uint64_t temp=curr_batch[i];

      temp=temp/P;  // unary value is quotient


      if(temp>delta_zero_count) // all 0's haven't been written yet.
      {
        bb_temp.bitset_write_bits(offset_bits,0,1);
        offset_bits++;
        delta_zero_count++;
        i--;    // keep writing 0's until the full value is written
      }
      else
      {
        bb_temp.bitset_write_bits(offset_bits,1,1); // end the unary value with '1'
        offset_bits++;
      }

    }

    return ;
  }


  //Creates the array of bit vectors
  //It batches values corresponding to each bit block and then calls "create_new_gcs_block" to create the actual blocks
  uint64_t build_bb(vector<uint64_t> &temp_locations)
  {
    vector<uint64_t> curr_batch;
    uint64_t num_batches=ceil(temp_locations.size()*1.00/block_size);

    uint64_t curr_index=0;
    uint64_t total_bits_used=0;
    uint64_t curr_size=0;


    for(int i=0;i<num_batches;i++)
    {
      curr_batch.resize(0); // initialize bit set

      int j=curr_index;
      uint64_t lower=(i*block_size*P);      // lower bound for bit indices in current batch
      uint64_t upper=((i+1)*block_size*P);  // upper bound for bit indices in current batch

      // while there are still locations to set and it is within the range of upper and lower bounds.
      while(j<temp_locations.size() && lower<=temp_locations[j] && temp_locations[j]<upper )
      {
        curr_batch.push_back(temp_locations[j]-lower);  // add normalized location (relative to start of batch) to current batch
        j++;
      }
      curr_index=j;
      create_new_gcs_block(curr_batch,bb_bitset_vec[i]);    // encode block using Golomb-coding

      vec_num_keys.push_back(curr_batch.size());    // track number of keys added to the current block

    }



    return total_bits_used;

  }

  
  //initialize snarf 
  void snarf_init(vector<T> &keys,double bits_per_key,int num_ele_per_block)
  {

    bool testbool = (bits_per_key>3);
    assert(("Bits per Key are too low!", testbool));

    double target_fpr=pow(0.5,bits_per_key-3.0);    // target false positive rate
    //Set parameter values
    N=keys.size();
    P=pow(2,ceil(log2(1.00/target_fpr)));
    bit_size=ceil(log2(1.00/target_fpr));
    block_size=num_ele_per_block;
    total_blocks=ceil(N*1.00/block_size);
    bb_bitset_vec.resize(total_blocks); // vector of bitsets initialized to `total_blocks` size
    
    //build snarf model
    rmi=snarf_model<T>();
    rmi.snarf_model_builder(keys);

    //Get bit locations of set bits
    vector<uint64_t> temp_locations;
    get_locations(keys,temp_locations);

    //Build bit blocks using the set bit location values
    gcs_size=build_bb(temp_locations);


    return ;
  }



  //checks if there is a value in a certain block(var bb_temp) that is between low_val and upper_val
  bool range_query_in_block(uint64_t low_val,uint64_t upper_val,snarf_bitset &bb_temp,int num_keys_read)
  {
    int num_keys=num_keys_read; // number of keys to look for in this block
    int delta_zero_count=0,delta_one_count=0;
    // offset_dense_itr used for reading unary codes, offset_sparse_itr used for reading binary codes
    int offset_dense_itr=num_keys*bit_size, offset_sparse_itr=0;

    int bit_val;    // used to store the unary bit value
    uint64_t temp;  // used to hold the re-constructed value of the location
    for(int i=0;i<num_keys;i++)
    {
      bit_val=bb_temp.bitset_read_bit(offset_dense_itr,1);  // read first unary bit value
      
      // bit_val==1 means end of unary code
      if(bit_val==1 && ((delta_zero_count+1)*P>=low_val) && ((upper_val)>=delta_zero_count*P))
      {
        //calculate the value (unary + binary)
        temp=delta_zero_count*P+bb_temp.bitset_read_bits(offset_sparse_itr,bit_size);
        
        //value is between the range
        if(temp>=low_val && temp<=upper_val)
        {
          return true;  // found a key location between the location ranges
        }
        
      }
      delta_zero_count+=(1-bit_val);    // adds 1 if bit_val==0 (i.e. not at end of unary code), and does nothing if bit_val==1 (i.e. at the end of unary code)
      offset_dense_itr++;   // move to next unary code bit index
      i-=(1-bit_val);       // if bit_val==1 then i is not decremented, otherwise, decrement to continue reading unary code in the current loop
      offset_sparse_itr+=(bit_val*bit_size);    // move to next binary code

    }

    return false;   // no keys found within the range, return false
  }
 

  //finds the bit location corresponding to the query endpoints and checks the corresponding block or blocks for a value
  bool range_query(T lower_val,T upper_val)
  {
    uint64_t temp_loc_lower,temp_loc_upper,large_delta_query_index;

    query_cdf1=rmi.infer(upper_val);    // cdf of lower range key
    query_cdf2=rmi.infer(lower_val);    // cdf of upper range key

    temp_loc_upper=floor(query_cdf1*N*P);   // estimated location of upper key
    temp_loc_upper=min(N*P-1,max((uint64_t)0,temp_loc_upper));  // keep the key location within the bounds of the bit array

     
    temp_loc_lower=floor(query_cdf2*N*P);   // estimated location of lower key
    temp_loc_lower=min(N*P-1,max((uint64_t)0,temp_loc_lower));  // keep the key location within the bounds of the bit array

    delta_query_index=floor(temp_loc_lower*1.00/(block_size*P));    // block index of the lower key
    large_delta_query_index=floor(temp_loc_upper*1.00/(block_size*P));  // block index of the upper key

    //in case the query endpoints are  in two different blocks we need to query multiple times
    if(delta_query_index==large_delta_query_index)
    {
      // only one block needs to be searched
      return range_query_in_block(
        temp_loc_lower-delta_query_index*block_size*P,  // normalized lower value location
        temp_loc_upper-delta_query_index*block_size*P,  // normalized upper value location
        bb_bitset_vec[delta_query_index],   // block to search
        vec_num_keys[delta_query_index]     // number of keys in block
    );
    }
    else
    {
      // need to search multiple blocks
      // first query block containing lower value
      if(
        range_query_in_block(
          temp_loc_lower-delta_query_index*block_size*P-1,
          block_size*P+1,
          bb_bitset_vec[delta_query_index],
          vec_num_keys[delta_query_index]
        )
      )
      {
        return true;
      }

      // next, query block containing upper value
      if(
        range_query_in_block(
            0,
            temp_loc_upper-large_delta_query_index*block_size*P,
            bb_bitset_vec[large_delta_query_index],
            vec_num_keys[large_delta_query_index]
        )
      )
      {
        return true;
      } 

      // check blocks between blocks containing lower and upper values
      for(int i=delta_query_index+1;i<large_delta_query_index;i++)
      {
        if(
          range_query_in_block(
            0,
            block_size*P-1,
            bb_bitset_vec[i],
            vec_num_keys[i]
          )
        )
        {
          return true;
        }
      }

      return false; // key not found in any of the multiple blocks

    }

    return false;   // not needed because both conditionals have returns

  }

  //returns the space used by snarf overall
  int return_size()
  {
    int total_size=0;
    total_size+=sizeof(name_curr);
    total_size+=rmi.return_size();
    total_size+=7*sizeof(N);

    for(int i=0;i<vec_num_keys.size();i++)
    {
      total_size+=sizeof(vec_num_keys[i]);
    }

    for(int i=0;i<bb_bitset_vec.size();i++)
    {
      total_size+=bb_bitset_vec[i].return_size();
    }

    return total_size;
  }


};

