myfunc = (a:string, b:string):int => {
  print(a);
  print("\n");
  print(b);
  print("\n");
  return 0;
};

main = (argc: int, argv:Array<string>):int => {
  myfunc("hello", "world");
  return 0;
};
