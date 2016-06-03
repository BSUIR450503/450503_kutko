#include "libs.h"

#ifdef _WIN32
#ifdef __cplusplus 
extern "C" {         
#endif

__declspec(dllexport) BOOL read(Lab5 *data)
{
	BOOL readData;
	DWORD Num;
	data->Overlapped.Offset = 0;

	ReadFile(data->hFile, data->tempString, data->NumberOfBytes, NULL, &data->Overlapped);
	WaitForSingleObject(data->Overlapped.hEvent, 1000);
	readData = GetOverlappedResult(data->hFile, &data->Overlapped, &Num, FALSE);

	return readData;
}
__declspec(dllexport) BOOL write(Lab5 *data)
{
	BOOL writeData;
	DWORD NumberOfBytesTrasferred;
	data->Overlapped.Offset = data->posReadfile;

	WriteFile(data->hFile, data->tempString, data->Overlapped.InternalHigh, NULL, &data->Overlapped);
	WaitForSingleObject(data->Overlapped.hEvent, 1000);
	writeData = GetOverlappedResult(data->hFile, &data->Overlapped, &NumberOfBytesTrasferred, FALSE);

	if (writeData) data->posReadfile = data->posReadfile + NumberOfBytesTrasferred;
	return writeData;
}

#ifdef __cplusplus
}
#endif
#endif

#ifdef __linux__
void asyncronicRead(struct Lab5 *data) {
  printf("\n\nReading from %s...\n",data->readFileName);

  int file = open(data->readFileName, O_RDONLY, 0);
  if (file == -1) {
    printf("Can't to open file for read!");
    return;
  }

  data->readCb.aio_nbytes = sizeof(data->buffer);
  data->readCb.aio_fildes = file;
  data->readCb.aio_offset = 0;
  data->readCb.aio_buf = data->buffer;
  
  aio_read(&data->readCb);
  while(aio_error(&data->readCb) == EINPROGRESS);
  data->bytesRead = aio_return(&data->readCb);
  close(file);
}

void asyncronicWrite(struct Lab5 *data) {

 int file = open(data->writeFileName, O_CREAT | O_RDWR | O_APPEND, 0666);
  if (file == -1) {
    printf("Can't to open file for write!");
    return;
  }

  data->writeCb.aio_nbytes = data->bytesRead;
  data->writeCb.aio_fildes = file;
  data->writeCb.aio_buf = data->buffer;
  
  aio_write(&data->writeCb);
  while(aio_error(&data->writeCb) == EINPROGRESS);
  data->bytesWrite = aio_return(&data->writeCb);
  close(file);
}
#endif