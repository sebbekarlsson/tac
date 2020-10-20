hello = ():int => {
  print("Hello!\n");
  return 1;
};

main = (argc: int, argv: Array<string>):int => {
  hello();
  return 16;
};
