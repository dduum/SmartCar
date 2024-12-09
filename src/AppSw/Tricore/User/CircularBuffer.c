/*
 * CircularBuffer.c
 *
 *  Created on: 2024��4��25��
 *      Author: dduu
 */
#include <CircularBuffer.h>

CircularBuffer bf;

// ��ʼ�����λ�����
void CircularBuffer_Init(CircularBuffer* cb) {
    cb->head = 0;
    cb->tail = 0;
}


// �жϻ��λ������Ƿ�����
int CircularBuffer_IsFull(const CircularBuffer* cb) {
    if((cb->head + 1) % BUFFER_SIZE == cb->tail){
        return 1;
    }else{
        return 0;
    }
}

// �жϻ��λ������Ƿ�Ϊ��
int CircularBuffer_IsEmpty(const CircularBuffer* cb) {
    if(cb->head == cb->tail){
        return 1;
    }else{
        return 0;
    }
}

// ���λ�����д������
int CircularBuffer_Write(CircularBuffer* cb, uint8 data) {
  if (CircularBuffer_IsFull(cb)) {  // �������������޷�д��
    return 0;
  }

  cb->buffer[cb->head] = data;
  cb->head = (cb->head + 1) % BUFFER_SIZE;
  return 1;
}

// �ӻ��λ�������ȡ����
int CircularBuffer_Read(CircularBuffer* cb, uint8* data) {
  if (CircularBuffer_IsEmpty(cb)) {  // ������Ϊ�գ������ݿɶ�ȡ
    return 0;
  }

  *data = cb->buffer[cb->tail];
  cb->tail = (cb->tail + 1) % BUFFER_SIZE;
  return 1;
}

// ��ȡ���λ�����ʣ���С
uint16 CircularBuffer_GetRemainingSize(const CircularBuffer* cb) {
  if (cb->head >= cb->tail) {
    return BUFFER_SIZE - (cb->head - cb->tail);
  } else {
    return cb->tail - cb->head - 1;
  }
}

// ��ȡ���λ�������д���С
uint16 CircularBuffer_GetWrittenSize(const CircularBuffer* cb) {
  if (cb->head >= cb->tail) {
    return cb->head - cb->tail;
  } else {
    return BUFFER_SIZE - (cb->tail - cb->head - 1);
  }
}

// �ӻ��λ�������ȡָ�����ȵ�����
int CircularBuffer_ReadData(CircularBuffer* cb, uint8* data, uint16 length) {
  if (CircularBuffer_GetWrittenSize(cb) < length) {
    return 0;  // �������е����ݲ���
  }

  for (uint16 i = 0; i < length; ++i) {
    if (!CircularBuffer_Read(cb, &data[i])) {
      return 0;  // ��ȡ���ݳ���
    }
  }

  return 1;
}

// ���λ�����д��ָ�����ȵ�����
int CircularBuffer_WriteData(CircularBuffer* cb, const uint8* data, uint16 length) {
  if (CircularBuffer_GetRemainingSize(cb) < length) {
    return 0;  // ������ʣ��ռ䲻��
  }

  for (uint16 i = 0; i < length; ++i) {
    if (!CircularBuffer_Write(cb, data[i])) {
      return 0;  // д�����ݳ���
    }
  }

  return 1;
}


// ʾ����STM32���ڽ����жϴ�����
//void USART_Receive_IRQHandler(void) {
//  uint8 data = USART_ReceiveData(USART1);  // ��ȡ���յ�������
//  if (!CircularBuffer_Write(&rxBuffer, data)) {
//    // �������������������
//  }
//}






