main = (argc: int, argv: Array<string>):int => {
  res:int = (5 + 5 * 5) - 1;
  value:string = itos(res, " ");
  print(value);
  return 0;
}
