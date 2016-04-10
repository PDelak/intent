intent
=======

Intent is a programming language which aims is simplicity of domain specific languages creation. Main feature of the language is its extensibility, virtually it can parse any context free grammar. 

### Hello world

	@print "hello world"

### Variables declaration

	let @x = 1

* all identifiers starts with @ sign (this is intentional to resolve keywords disambiquities)

@x type will be inferred as int and is a shortcut for following declaration

	let @x : int = 1

### Types 

List of currently supported types :
 * `string` 
 * `int`
 * `bool` 
 * `table`

### Builtin statements

* `let`

		let @x = 1

* `if statement`

		if (@x == 1) {} 
		else if(@x == 2) {}
		else {}

* `Loops`

		while (@y == 1) {}

		for( @x = 1, 5, 1) {}

### Extensions

In order to extend a language with specific grammar, a typeset has to be defined. Typeset is a kind of grammar plugin.
Let's say that we would like to have a way to define state machines in a declarative way

	statemachine basic
	{
  		initialState : below5

	    state below5 
	    {
		    transition : above5 when >= 5
		
		    onEnter(below5)
		    {      
		      @print("Entering below5")
		    }
		    onExit(below5)
		    {
		      @print("Exitting below5")      
		    }
		    onRead(below5)
		    {
		      @print("Reading below5")      
		    }
	  	}

		state above5 
		{
		    transition : below5 when < 5

		    onEnter(above5)
		    {      
		      @print("Entering above5")
		    }
		    onExit(above5)
		    {
		      @print("Exitting above5")      
		    }
		    onRead(above5)
		    {
		      @print("Reading above5")      
		    }
		}
	}

To handle such definition following typeset needs to be added

	typeset statemachine = [
	  type main = stm stm_processor?;
	  type stm = 'statemachine' "[a-zA-Z0-9]*" '{' config state* '}';
	  type config = 'initialState' ':' "[a-zA-Z0-9]*";
	  type state = 'state' "[a-zA-Z0-9]*" '{' transition entry* '}';
	  type entry = (onEnter | onExit | onRead);
	  type onEnter = 'onEnter' '(' "[a-zA-Z0-9]*" ')' '{' statement* '}';
	  type onExit = 'onExit' '(' "[a-zA-Z0-9]*" ')' '{' statement* '}';
	  type onRead = 'onRead' '(' "[a-zA-Z0-9]*" ')' '{' statement* '}';
	  type stm_number = "[0-9]*";
	  type stm_operator = '==' | '>=' | '<' | '>' | '<=';
	  type transition = 'transition' ':' "[a-zA-Z0-9]*" 'when' stm_operator stm_number;
	  type stm_processor = 'stm_process' identifier "[a-zA-Z0-9]*"; 
	]

After that your problem will "eat" grammar, but there is one additional step needed to have a working example - definition of code generator.
To do that, we have to define set of matchers that will match specific productions. Below onEnter match definition.

	match(onEnter)
	{
	   |=> {
	      function @onEnter_$2.value() 
	      {      
	        $5.value      
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

Referencing to node is not checked against its availability. For instance referencing to $3 in below
definition will lead to segfault.
		

		
	type stm = stm stm_processor?;



