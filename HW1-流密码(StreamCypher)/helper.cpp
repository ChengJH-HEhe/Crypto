#include <cstring>
#include <string>


using namespace std;

/*
一些约定：加密前的信息称为明文，加密后的信息称为密文

HINT0: 本题目是一个简单的流密码破解题目，题意在requirement.md中已说明
HINT1: 假设A，B两明文key加密后密文分别是 EA = A^key, EB = B^key, 那么 EA^EB = A^B. 即：明文异或等于密文异或
HINT2: 空格^a = A, 空格^A = a;（异或空格后实现大小写字母转换）
HINT3: (空格^key)^空格 = key (知道明文第i位是空格后，密文第i位^空格 = 密钥)
HINT4: 解密后的文本可能需要修改一些奇怪（?）的字符获得对应明文，
*/ 


// 16进制二位数转10进制二位数
int hex2dec2(char a, char b) {
  int x = a - '0';
  if (!(x >= 0 && x <= 9))
    x = (a - 'a' + 10);
  int y = b - '0';
  if (!(y >= 0 && y <= 9))
    y = (b - 'a' + 10);
  // printf("a:%c b:%c -> %d\n", a, b, x * 16 + y);
  return x * 16 + y;
}

/*
  题意中所给的10个密文以及最终的目标字符串
  值得注意的是，密文是以16进制编码，两位代表一个ASCII码。
*/
string s[] = {

    "315c4eeaa8b5f8aaf9174145bf43e1784b8fa00dc71d885a804e5ee9fa40b16349c146fb778cdf2d3aff021dfff5b403b510d0d0455468aeb98622b137dae857553ccd8883a7bc37520e06e515d22c954eba5025b8cc57ee59418ce7dc6bc41556bdb36bbca3e8774301fbcaa3b83b220809560987815f65286764703de0f3d524400a19b159610b11ef3e",
    "234c02ecbbfbafa3ed18510abd11fa724fcda2018a1a8342cf064bbde548b12b07df44ba71"
    "91d9606ef4081ffde5ad46a5069d9f7f543bedb9c861bf29c7e205132eda9382b0bc2c5c4b"
    "45f919cf3a9f1cb74151f6d551f4480c82b2cb24cc5b028aa76eb7b4ab24171ab3cdadb835"
    "6f",
    "32510ba9a7b2bba9b8005d43a304b5714cc0bb0c8a34884dd91304b8ad40b62b07df44ba6e"
    "9d8a2368e51d04e0e7b207b70b9b8261112bacb6c866a232dfe257527dc29398f5f3251a0d"
    "47e503c66e935de81230b59b7afb5f41afa8d661cb",
    "32510ba9aab2a8a4fd06414fb517b5605cc0aa0dc91a8908c2064ba8ad5ea06a029056f47a"
    "8ad3306ef5021eafe1ac01a81197847a5c68a1b78769a37bc8f4575432c198ccb4ef635902"
    "56e305cd3a9544ee4160ead45aef520489e7da7d835402bca670bda8eb775200b8dabbba24"
    "6b130f040d8ec6447e2c767f3d30ed81ea2e4c1404e1315a1010e7229be6636aaa",
    "3f561ba9adb4b6ebec54424ba317b564418fac0dd35f8c08d31a1fe9e24fe56808c213f17c"
    "81d9607cee021dafe1e001b21ade877a5e68bea88d61b93ac5ee0d562e8e9582f5ef375f0a"
    "4ae20ed86e935de81230b59b73fb4302cd95d770c65b40aaa065f2a5e33a5a0bb5dcaba437"
    "22130f042f8ec85b7c2070",
    "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd2061bbde24eb76a19d84aba34"
    "d8de287be84d07e7e9a30ee714979c7e1123a8bd9822a33ecaf512472e8e8f8db3f9635c19"
    "49e640c621854eba0d79eccf52ff111284b4cc61d11902aebc66f2b2e436434eacc0aba938"
    "220b084800c2ca4e693522643573b2c4ce35050b0cf774201f0fe52ac9f26d71b6cf61a711"
    "cc229f77ace7aa88a2f19983122b11be87a59c355d25f8e4",
    "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd90f1fa6ea5ba47b01c909ba76"
    "96cf606ef40c04afe1ac0aa8148dd066592ded9f8774b529c7ea125d298e8883f5e9305f4b"
    "44f915cb2bd05af51373fd9b4af511039fa2d96f83414aaaf261bda2e97b170fb5cce2a53e"
    "675c154c0d9681596934777e2275b381ce2e40582afe67650b13e72287ff2270abcf73bb02"
    "8932836fbdecfecee0a3b894473c1bbeb6b4913a536ce4f9b13f1efff71ea313c8661dd9a4"
    "ce",
    "315c4eeaa8b5f8bffd11155ea506b56041c6a00c8a08854dd21a4bbde54ce56801d943ba70"
    "8b8a3574f40c00fff9e00fa1439fd0654327a3bfc860b92f89ee04132ecb9298f5fd2d5e4b"
    "45e40ecc3b9d59e9417df7c95bba410e9aa2ca24c5474da2f276baa3ac325918b2daada43d"
    "6712150441c2e04f6565517f317da9d3",
    "271946f9bbb2aeadec111841a81abc300ecaa01bd8069d5cc91005e9fe4aad6e04d513e96d"
    "99de2569bc5e50eeeca709b50a8a987f4264edb6896fb537d0a716132ddc938fb0f836480e"
    "06ed0fcd6e9759f40462f9cf57f4564186a2c1778f1543efa270bda5e933421cbe88a4a522"
    "22190f471e9bd15f652b653b7071aec59a2705081ffe72651d08f822c9ed6d76e48b63ab15"
    "d0208573a7eef027",
    "466d06ece998b7a2fb1d464fed2ced7641ddaa3cc31c9941cf110abbf409ed39598005b339"
    "9ccfafb61d0315fca0a314be138a9f32503bedac8067f03adbf3575c3b8edc9ba7f5375305"
    "41ab0f9f3cd04ff50d66f1d559ba520e89a2cb2a83","32510ba9babebbbefd001547a810e67149caee11d945cd7fc81a05e9f85aac650e9052ba6a8cd8257bf14d13e6f0a803b54fde9e77472dbff89d71b57bddef121336cb85ccb8f3315f4b52e301d16e9f52f904"};

int cur; // 当前处理的密文 
int cnt[11][1000]; // cnt[cur][i]表示第cur个密文的第i个字符可能是空格的次数
int msg[1000]; // msg[i]表示第i个字符的key是 s[msg[i]][i:i+1]^空格
// xor s1 & s2, check whether s1[cur][i:i+1] is alpha
void xor_strings(string s1, string s2) {
  for (int i = 0; i < min(s1.size(), s2.size()); i+=2) {
    // res = s1[i:i+1] ^ s2[i:i+1]
    int res = (hex2dec2(s1[i], s1[i+1]) ^ hex2dec2(s2[i], s2[i+1]));
    if(res == char(0) || (res >= 'a' && res <= 'z') || (res >= 'A' && res <= 'Z')) {
      // TODO: if res is alpha or 0, s1[i:i+1] is likely to be SPACE, you need to update cnt[cur][i]
      
    }
  }
}


int main() {
  for(int i = 0; i < s[10].size(); i+=2) {
    msg[i] = -1;
  }
  for (cur = 0; cur <= 9; ++cur){
    for (int k = 0; k <= 9; ++k) {
      // printf("[%d] ^ [%d]:\n\t", cur, k);
      if(cur == k) continue;
      xor_strings(s[cur], s[k]);
      // printf("\n");
    }
    // TODO: update msg[i] according to cnt[cur][i], you can set a threshold value to determine the key
  
  }
  // print the flag
  for(int i = 0; i < s[10].size(); i+=2) {
    if(msg[i] == -1) {
      printf("%c", hex2dec2(s[10][i], s[10][i+1]));
      continue;
    }
    printf("%c", char(hex2dec2(s[10][i], s[10][i+1]) ^ hex2dec2(s[msg[i]][i], s[msg[i]][i+1]) ^ 0x20));
  }
}