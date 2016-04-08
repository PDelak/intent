intent
=======

Intent is a programming language which aims is simplicity of domain specific languages creation. Main feature of the language is its extensibility, virtually it can parse any context free grammar. 

### Hello world

	@print "hello world"

### Ordinary stuff

Variables declaration

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
