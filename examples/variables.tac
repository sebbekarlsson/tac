hello = (x: string, y:string) => {
  print(x);
  return 0;
};


main = (argc: int, argv: Array<string>):int => {
  abc:string = "hello world this is the tac language and this is a very long string\n";
  xyz:string = "this is xyz\n";
  hello(xyz);
  return 0;
};
