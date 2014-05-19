function myPow(a,b) 
{
    a = parseInt(a);
    b = parseInt(b);
    var result =a;
    for(var i=1;i<b;i++) {
      result*=a;
    }
    return result;
}