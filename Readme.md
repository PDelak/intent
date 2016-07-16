intent
=======

Intent is a programming language which aims is simplicity of domain specific languages creation. Main feature of the language is its extensibility, virtually it can be extended and parse any context free grammar. Besides of that, it is small and highly embeddable.
For now, there are several limitations in order to build fully functional programs (look at Known problems and limitations), on the other hand
it has implemented all elements to prove that approach makes sense.

### Hello world

	print ("hello world");

### Variables declaration

	let x = 1;

x type will be inferred as int and is a shortcut for following declaration

	let x : int = 1;

### Types 

List of currently supported types :
 * `string` 
 * `int`
 * `bool` 
 * `table`

### Builtin statements

* `let`

		let x = 1;

* `if statement`

		if (x == 1) {} 
		else if(x == 2) {}
		else {}

* `Loops`

		while (y == 1) {}

		for( x = 1, 5, 1) {}

### Extensions

In order to extend a language with specific grammar, a grammar plugin has to be defined. 
Let's say that we would like to have a way to define state machines in a declarative way

	statemachine basic
	{
	  initialState : below5

	  state below5 {
	    transition : above5 when >= 5
	    onEnter
	    {      
	      print("Entering below5");
	    }
	    onExit
	    {
	      print("Exitting below5");
	    }
	    onRead
	    {
	      print("Reading below5");
	    }
	  }

	  state above5 {
	    transition : below5 when < 5
	    onEnter
	    {      
	      print("Entering above5");
	    }
	    onExit
	    {
	      print("Exitting above5");
	    }
	    onRead
	    {
	      print("Reading above5");
	    }
	  }
	}

To handle such definition following grammar needs to be added

	grammar statemachine = [
	  syntax main = stm stm_processor?;
	  syntax stm = 'statemachine' "[a-zA-Z0-9]*" '{' config state* '}';
	  syntax config = 'initialState' ':' "[a-zA-Z0-9]*";
	  syntax state = 'state' "[a-zA-Z0-9]*" '{' transition entry* '}';
	  syntax entry = (onEnter | onExit | onRead);
	  syntax onEnter = 'onEnter' '(' "[a-zA-Z0-9]*" ')' '{' statement* '}';
	  syntax onExit = 'onExit' '(' "[a-zA-Z0-9]*" ')' '{' statement* '}';
	  syntax onRead = 'onRead' '(' "[a-zA-Z0-9]*" ')' '{' statement* '}';
	  syntax stm_number = "[0-9]*";
	  syntax stm_operator = '==' | '>=' | '<' | '>' | '<=';
	  syntax transition = 'transition' ':' "[a-zA-Z0-9]*" 'when' stm_operator stm_number;
	  syntax stm_processor = 'stm_process' identifier "[a-zA-Z0-9]*"; 
	]

After that your program will "eat" grammar, but there is one additional step needed to have a working example - definition of code generator.
To do that, we have to define set of matchers that will match specific productions. Below onEnter match definition.

	match(onEnter)
	{
	   |=> {
	      "onEnter" = function () 
	      {      
	        $2.value      
	      }
	   }
	}

### Platforms

Currently intent is compiling on windows, but it written in standard c++ so there should not be a problem to port it to any other platform

### Current work

 * `stability`
 * `compiler diagnostic (better error messages)`

### Future work 

 * `functions as first class citizens`
 * `reflection mechanism` 

### Dependencies
intent depends on three 3rd party libraries

 * `dparser - sglr parser (i'm considering to write new parser engine from scratch due to some limitations, but that's future)`
 * `lexertl - lexer library` 
 * `lua - used as a backend` 

 ### Known problems and limitations
 * `reference usage can lead to segfault`. 

Referencing to node is not checked against its availability. For instance referencing to $2 in below
definition will lead to segfault (stm production contains only two nonterminals $0 and $1).
		

		
	syntax stm = stm stm_processor?;


 * `line numbers in the case of parsing errors are missleading`

* `function can not be passed to a table as value`  

Due to this limitation, some features can not be implemented (dynamic polymorphism).
