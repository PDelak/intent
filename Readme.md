intent
=======

Intent is a simple programming language which aims simplicity of domain specific languages creation. Language is exensible 
and can process any context free grammar

### Hello world

	@print "hello world"

### Ordinary stuff

Variables declaration

	let @x = 1

* all identifiers starts with @ sign

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
