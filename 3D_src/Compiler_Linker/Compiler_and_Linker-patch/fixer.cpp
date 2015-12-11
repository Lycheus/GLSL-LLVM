#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
using namespace std;

bool IsChar( char c );
bool IsDigit( char c );
size_t find_last_right_brace( string back_up );
enum Type{
  Int,
  Float,
  Vector,
  Array,
  Struct
};

struct SaveVariable{
  string type_name;
  string name;
  Type type;
  int index;
};

int main( int argc , char** argv){

  fstream inputfile,outputfile;
  char buffer;
  string NewVariable; //The new string we create will replace the original string in file.
  string output;
  map<string,string> MapOldStringToNewString; 
  map<string,string>::iterator it;
  SaveVariable SavedVariable[100];
  int mark = 0;
  int i = 0;
  int flag = -1;
  string PutStringIntoSystem;
  char *p;
  int arraysize;
  int Max = 0; //Save the max size of array.
  bool IsArray = false;
  int counter_for_if=0;
  int counter_for_discard=0;
  int counter_for_FragColor=0;

  p = strrchr( argv[1] , '.' );
  
  if( argc != 2 ){
    cerr<< "Error. Have to input the filename\n";
    exit(1);
  }

  if ( !p )
    flag = -1;
  else if( !strcmp( p , ".vert") ){ //Determine this file is vertex shader or fragment.
    MapOldStringToNewString[ "gl_Position" ] = "Position_temp";
    output += "vec4 Position_temp;\n";    
    flag = 0;
  }
  else  if( !strcmp( p , ".frag") ){
    MapOldStringToNewString[ "gl_FragColor" ] = "FragColor_temp";
    output += "vec4 FragColor_temp;\n";
    flag = 1;
  }
    
  if( flag == -1 ){
    cout << "The format of filename is not specified.\n";
    cout << "Filename of vertex shader: *.vert\n";  
    cout << "Filename of fragment shader: *.frag\n";
    exit(1);
  }

  PutStringIntoSystem  +=  "cat ";
  PutStringIntoSystem  +=  argv[1];
  PutStringIntoSystem  +=  " | tr -d \'\\r\' > ";
  PutStringIntoSystem  +=  argv[1];
  PutStringIntoSystem  +=  "_1";
  PutStringIntoSystem  +=  "; mv ";
  PutStringIntoSystem  +=  argv[1];
  PutStringIntoSystem  +=  "_1 ";
  PutStringIntoSystem  +=  argv[1];
  system( PutStringIntoSystem.c_str() );
  /*
  PutStringIntoSystem.clear(); //temporarily remove mediump, highp and lowp.
  PutStringIntoSystem += "sed \'s/mediump//g\' ";
  PutStringIntoSystem += argv[1];
  PutStringIntoSystem += "> new.vert6";
  system( PutStringIntoSystem.c_str() );
  PutStringIntoSystem.clear();
  PutStringIntoSystem += "sed \'s/highp//g\' new.vert6 > new.vert7";
  system( PutStringIntoSystem.c_str() );
  PutStringIntoSystem.clear();
  PutStringIntoSystem += "sed \'s/lowp//g\' new.vert7 >";
  PutStringIntoSystem += argv[1];
  cout<<PutStringIntoSystem<<endl;
  system( PutStringIntoSystem.c_str() );
  */


  
  inputfile.open( argv[1] );

  outputfile.open( "new.vert" , ios::out  );
  /*
  inputfile.seekg( 0 , ios::end );
  int length = inputfile.tellg();
  inputfile.seekg( 0 , ios::beg );
  */
  //cout << length << endl;

  if( !inputfile ){
    cout<<"Opening file is failed\n";
    exit(1);
  }

  while( ( buffer = inputfile.get() ) != EOF && !inputfile.eof()/*&& length != inputfile.tellg() */){

    //cout<< "Come entry\n";
    if( IsDigit( buffer ) ){
      string temp;
      do{
	temp += buffer;
	buffer = inputfile.get();
	if( buffer == '.' ){
	  temp += buffer;
	  buffer = inputfile.get();
	}

      }while( IsDigit( buffer) );
      output += temp;
      //cout<<temp<<endl;
    }
    
    else if( IsChar( buffer ) ){
      string temp;
      do{
	
	temp += buffer;
	buffer = inputfile.get();

      }while( IsChar( buffer ) );
      
      //cout<< temp <<endl;

      if( flag == 0 ){ // Run the vertex shader code.
	it = MapOldStringToNewString.find(temp); // Replace the original string.
	if( it != MapOldStringToNewString.end() )
	  output += it->second;
	else
	  output += temp;
	
	if( buffer == '[' && mark == 2 ){   //Handle the problem of the array.
	  IsArray = true;
	  output += buffer;

	  
	  while( ( buffer = inputfile.get() ) == ' ' ) 
	    ;
	  
	  output += buffer;
	  arraysize = buffer;
	  
	  
	  while( ( buffer = inputfile.get() ) == ' ' )
	    ;

	  output += buffer;

	  buffer = inputfile.get();


	  SavedVariable[i].type = Array;
	  SavedVariable[i].index = arraysize;

	}
	else if( buffer == '[' && ( it != MapOldStringToNewString.end() ) ){

	  output += buffer;
	  while( ( buffer = inputfile.get() ) == ' ' )
	    ;

	   int temp_size = buffer - 48;
	   if( temp_size > Max )
	     Max = temp_size;
	}
	
	if( !strcmp( temp.c_str() , "varying") ){
	  mark = 1;
	}
	else if( mark == 1 ){
	  SavedVariable[i].type_name = temp;
	  //cout<<"temp: " <<temp <<endl;
	  //cout<<"Saved: " <<SavedVariable[i].type_name<<endl;
	  if( !strncmp( temp.c_str() , "vec" , 3 ) )
	    SavedVariable[i].type = Vector;
	  else if( !strcmp( temp.c_str() , "float" ) )
	    SavedVariable[i].type = Float;
	 
	  mark++;
	}
	else if( mark == 2 ){
	 
	  SavedVariable[i].name = temp;
	  NewVariable = "temp_var" ;
	  char varying_index[3];
	  sprintf(varying_index,"%d",i);
	  NewVariable += varying_index;
	  cout<<"Add information about : "<< temp << " => " << NewVariable <<endl;
	  MapOldStringToNewString[ temp ] = NewVariable;

	  if( IsArray ){
	    NewVariable += "[";
	    NewVariable += arraysize;
	    NewVariable += "]";
	    IsArray = false;
	  }

	  output+= buffer;
	  output+="\n";
	  output+=SavedVariable[i].type_name;
	  output+=" ";
	  output+=NewVariable;
	  output+=";";

	  mark = 0;
	  i++;
	  continue;
	}

      }
      else { // Run the fragment shader code.
	//****************For discard instruction.//Deal the case which keyword "discard" is next line of gl_FragColor. //Discard appear. FS fragment should not write any color into buffer.
	if(temp=="if"){
	  counter_for_if++;
	}
	if(temp=="discard"){
	  string buffer1;
	  counter_for_discard++;
	  if(counter_for_if==0&&counter_for_FragColor!=0){
	    size_t found1 =  output.find("main");
	    if (found1!=string::npos){
	      found1 =  output.find("{",found1+1,1);
	      if (found1!=string::npos){
		output.replace(found1,1,"{\nbool k=true;\nif(k)\ndiscard;\n");
		temp = " ";
	      }
	    }
	  }
	}
	if(temp=="gl_FragColor"){
	  counter_for_FragColor++;
	}
	//****************For discard instruction.
	it = MapOldStringToNewString.find( temp );
	if( it != MapOldStringToNewString.end() )
	  output += it->second;
	else
	  output += temp;
      }
    }
    
        
    //cout<< buffer << endl;
    output+=buffer;
    //cout<< inputfile.tellg() << endl;

  }
  //cout << temp << endl;
  //outputfile.seekg( 0 , ios::end  );
  //outputfile<<"f";
  //cout<< (int) found <<" "<< output[found]<<endl;
  string final;
  
  cout<<"There are "<< i <<" varying variables\n";
  
  int j = 0; // Variable i counts how many varying variable are.  
  while (j < i){

    if( SavedVariable[j].type == Array ){

      int index = 0; 

      while( index <= Max ){
	final += SavedVariable[j].name;
	final += '[';
	final += index+48;
	final +=  "] = " ;
	final += MapOldStringToNewString[ SavedVariable[j].name ];
	final += '[';
	final += index+48;
	final += "];\n}";
	index++;
      }
    }
    else
      final = SavedVariable[j].name + " = " + MapOldStringToNewString[ SavedVariable[j].name ]+ ";\n}";

    //size_t found = output.rfind("}"); //old method
    //New method
    size_t found;
    /*
    if( flag == 0 ){
      found = output.rfind("Position_temp");
      found = output.find( "}" , found+1 , 1);
    }
    else{
      found = output.rfind("FragColor_temp");
      found = output.find( "}" , found+1 , 1);
    }
    */
    found = find_last_right_brace(output);
    //cout << (int) found << endl;
    if( found != string::npos )
      output.replace( found , 1 , final); //New string named "temp_var" replace the right brace at the end of file. 
    //cout << final;
    //output += final; //Append the string. Like normal = temp_var5.
    final.clear();
    j++;
  }


  /* old method
  size_t found = output.rfind("}");
  if( found != string::npos)
    output.replace( found , 1 , "return;\n}");
  */
  //new mothod to find the last '}' in the main function.
  size_t found;
  /*
  if( flag == 0){
    found = output.rfind("Position_temp");
    found = output.find( "}" , found+1 , 1);
    if( found != string::npos)
      output.replace( found , 1 , "return;\n}" );
  }
  else{
    found = output.rfind("FragColor_temp");
    found = output.find( "}" , found+1 , 1);
    if( found != string::npos)
      output.replace( found , 1 , "return;\n}" );
      }*/
  found = find_last_right_brace(output);
  
  if( found != string::npos)
    output.replace( found , 1 , "return;\n}" );
  if( flag == 0 )
    final += "gl_Position = Position_temp";
  else 
    final += "gl_FragColor = FragColor_temp"; 

  found = -1;
  while( ( found = output.find( "return;" , found+1 , 7  ) ) != string::npos )
    output.replace( found , 6 , final );
  
  



  
  /*for ( it = MapOldStringToNewString.begin(); it != MapOldStringToNewString.end();it++) //Check the content of MapOldStringToNewString.
    cout<< (*it).first << " => " << (*it).second <<endl;
  */
  
  //cout << output;

  outputfile << output;

  inputfile.close();
  outputfile.close();
  /*
  PutStringIntoSystem.clear();
  //Bypass the precision syntax.
  PutStringIntoSystem += "sed \"/precision.*mediump.*float;/d\" new.vert > new.vert2";
  system( PutStringIntoSystem.c_str() );
  PutStringIntoSystem.clear();
  PutStringIntoSystem += "sed \"/precision.*mediump.*int;/d\" new.vert2 > new.vert3";
  system( PutStringIntoSystem.c_str() );
  PutStringIntoSystem.clear();
  PutStringIntoSystem += "sed \"/precision.*mediump.*sampler2D;/d\" new.vert3 > new.vert4";
  system( PutStringIntoSystem.c_str() );
  PutStringIntoSystem.clear();
  PutStringIntoSystem += "sed \"/precision.*mediump.*samplerCube;/d\" new.vert4 > new.vert5";
  system( PutStringIntoSystem.c_str() );

  PutStringIntoSystem.clear();  
  PutStringIntoSystem += "mv new.vert5 ";
  PutStringIntoSystem += argv[1];

  system( PutStringIntoSystem.c_str() );
*/
  PutStringIntoSystem.clear();  
  PutStringIntoSystem += "mv new.vert ";
  PutStringIntoSystem += argv[1];

  system( PutStringIntoSystem.c_str() );

  return 0;
}


bool IsChar( char c ){
  if( (c >= 'a' && c <= 'z') || (c >= 'A' && c<= 'Z') || (c >= '0' && c <= '9') ||   c == '_' )
    return true;
 
  return false;
}
bool IsDigit( char c ){
  if( c >= '0' && c <= '9' )
    return true;
 
  return false;
}
//Find the last right brace of main function.
size_t find_last_right_brace( string back_up){

  size_t found;
  int count = 0;
  found = back_up.find("main");
  found = back_up.find("{",found+1,1);
  count = 1;
  size_t size = back_up.size();
  found++;
  while( count > 0 || found < size){
    if( back_up[found] == '{' )
      count++;
    else if( back_up[found] == '}' ){
      count--;
      if( count == 0 )
	return found;
    }
    found++;
  }


}
