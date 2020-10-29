hello = (x: string, y: string, z: string, a: string, b: string, c:string, d:string) => {
  print(x);
  print(y);
  print(z);
  print(a);
  print(b);
  print(c);
  print(d);
  return 1;
};

main = (argc: int, argv: Array<string>):int => {
  hello("first is a bit longer than the other ones\n", "second\n", "third\n", "hello I am a bit long too, but hopefully it will work\n", "five\n", "six\n", "seven\n");
  return 0;
};
