hello = (x: string, y: string) => {
  print(x);
  print(y);
  return 1;
};

main = (argc: int, argv: Array<string>):int => {
  hello("a", "b");
  return 0;
};
