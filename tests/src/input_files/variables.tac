hello = (x: string, y:string, z:string) => {
  print(x);
  print(y);
  print(z);
  return 0;
};

/*
* main method
*/
main = (argc: int, argv: Array<string>):int => {
  foo:string = "the first string\n";
  bar:string = "the second string\n";
  far:string = "the third string\n";
  
  print(foo);
  print("yoyo\n");
  print(bar);
  print(far);
  value: string = itos(256, " ");
  print(value);
  print("\n");

  return 0;
};
