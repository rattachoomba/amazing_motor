// callback for received data
void receiveData(int byteCount){
  // Check byte[0] (its the command)
  if(byteCount > 1) {
    number_bytes = byteCount;
    if(Wire.available()){
      Wire.readBytes(number, byteCount);// read the data and store the digit array to number[]                    
      for (i = 0; i < 7; ++i){          // set a variable store the combine digits
          result = result*10+number[i]; 
          }                             
      Serial.print("\n"); 
      Serial.print("digit combined ");  
      Serial.println(result);           
      int divide=1000;                  //set three decimal places
      floatpt= (float)result/divide;    //floating point number with 3 decimal places
      Serial.println(floatpt,3);        
      result = 0;                       //reset the result to receive new data
    }
  }
  else {
    byte b = Wire.read();
  }
}


