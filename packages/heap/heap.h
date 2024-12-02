#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <stdint.h>

/* �ѵĴ�С(�ֽ�Ϊ��λ,��1024�ֽ�����ʱ����������� 8*1024 ����ʽ��ʾ 8KB) */
#define	heapTOTAL_HEAP_SIZE			(size_t)(8*1024)

/* �ֽڶ��볤��(1/2/4/8�ֽ�) */
#define heapALIGNMENT_BYTE			( 8 )

/* �ֽڶ������룬���ڼ��� */
#define heapALIGNMENT_MASK	        ( heapALIGNMENT_BYTE - 1 )

/* ������˳��ٽ�����Ҳ���Դ��������ź��� */
#define heapINTO_CRITICAL() //__disable_irq();
#define	heapEXIT_CRITICAL() //__enable_irq();

/* API���� */
void*  pvHeapMalloc( size_t xWantedSize );
void   vHeapFree( void *pv );
size_t xHeapGetFreeSize( void );
size_t xHeapGetMinimumEverFreeSize(void);

#endif /* MEMMANG_HEAP_H */
