# MikuMikuFormats

## �T�v

���̃��C�u������MikuMikuDance�Ŏg���Ă���ȉ��̃t�H�[�}�b�g�̓ǂݍ��݂��s���܂��B

- PMD
- PMX(Ver2.0; 2.1�ɂ͏����Ή�)
- VMD

## �K�v

�ȉ����g���Ă��邽�߁AC++11�ɑΉ������R���p�C�����K�v�ł��B

- scoped enumeration
- uint32_t,uint16_t,uint8_t
- unique_ptr

## �����R�[�h�̈����ɂ���
EncodingHelper.h����Win32API���g���ĕϊ����Ă��܂��B
���̂���Windows�ˑ��ł��B

## �T���v��
```cpp
const char *filename = "sample.pmx";
pmx::PmxModel model;
std::ifstream stream = std::ifstream(filename, std::ios_base::binary);
model.Read(&stream);
stream.close();
```

## ���C�Z���X

�����R�ɂ��g���������B
���̃v���O�����g�p�ɂ�邢���Ȃ鑹�Q���Aoguna�͐ӔC�𕉂�Ȃ����̂Ƃ��܂��B

Distributed under CC0 1.0 Universal.

