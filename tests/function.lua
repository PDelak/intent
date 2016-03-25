function foo(param)
	do
		print(param)
		return 5
	end
end
function foo2(param, param2)
	do
		print(param)
		print(param2)
		return true
	end
end
function foo3(param, param2, param3)
	do
		print(param)
		print(param2)
		print(param3)
		return "hello"
	end
end
foo(1)
foo2(2,true)
foo3(3,true,"hello")
