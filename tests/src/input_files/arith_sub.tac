hello = () => {
  print("hello!\n");
  return 0;
};

main = (argc: int, argv: Array<string>):int => {
  x:int = 16 - (6 - 3);
  value:string = itos(x, " ");
  print(value);
  print("\n");
  hello();
  return 0;
}
