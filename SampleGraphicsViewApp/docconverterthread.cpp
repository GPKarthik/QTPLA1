#include "docconverterthread.h"
#include <QCoreApplication>
#include <QStringList>
#include "customgraphicsview.h"
#include <Sddl.h>

DocConverterThread::DocConverterThread(QObject *parent)
							: QThread(parent)
{
}

DocConverterThread::~DocConverterThread()
{

}

void DocConverterThread::run()
{
	HRESULT hr = E_FAIL;

	SECURITY_ATTRIBUTES SecurityAttributes = {0};

	HANDLE hNotifyPipe = NULL;

	do 
	{
		QStringList args = QCoreApplication::arguments();

		if(2 != args.count())
		{
			break;
		}

		QString strCommandLine = args[1].trimmed();
		
		int nEventPosition = strCommandLine.lastIndexOf("\\");

		if (-1 == nEventPosition)
		{
			break;
		}

		QString strEvent = strCommandLine.mid(nEventPosition + 1);
		
		if (true == strEvent.endsWith(":"))
		{
			strEvent.remove(strEvent.length() - 1, 1);
		}

		strExtractedFolder = strCommandLine.mid(0, nEventPosition);

		if (false == strExtractedFolder.endsWith("\\"))
		{
			strExtractedFolder.append("\\");
		}

		QString strEventName = QString("Global\\%1").arg(strEvent);

		HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE,
								  FALSE,
								  strEventName.toStdWString().c_str());

		//Validate Handle.
		if (NULL == hEvent)
		{
			break;
		}

		LPCWSTR lpwcsSDDL = NULL;

		//Grant ALL Access Permissions to Everyone.
		lpwcsSDDL = L"D:"                 //Discretionary ACL.
					L"(A;OICI;GA;;;WD)";  //Allow full control. 

		//Convert SDDL to Security Descriptor.
		if (FALSE == ConvertStringSecurityDescriptorToSecurityDescriptor(
								lpwcsSDDL,
								SDDL_REVISION_1,
								&(SecurityAttributes.lpSecurityDescriptor),
								NULL))
		{
			break;
		}

		//Create a Pipe Name from the Event Name.
        QString strPipeName = QString("\\\\.\\pipe\\%1_Pipe").arg(strEvent);

		hNotifyPipe = CreateNamedPipe(
                        strPipeName.toStdWString().c_str(),          //pipe name.
                        PIPE_ACCESS_INBOUND,       //read/write access.
                        PIPE_TYPE_BYTE |          //binary type pipe.
                        PIPE_READMODE_BYTE |      //byte-read mode.
                        PIPE_WAIT,                //blocking mode.
                        1,                        //max. instances.  
                        0,						  //output buffer size.
                        sizeof(NOTIFYSTATE) +
						sizeof(FPNOTIFY),		  //input buffer size.
                        NMPWAIT_USE_DEFAULT_WAIT, //client time-out.
                        &SecurityAttributes);     //default security attribute.

		if (INVALID_HANDLE_VALUE == hNotifyPipe) 
		{
			break;
		}

		if (FALSE == SetEvent(hEvent))
		{
			break;
		}

		//Wait for the client to connect; if it succeeds, 
        //the function returns a nonzero value. If the function
        //returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 
        BOOL bConnected = ConnectNamedPipe(
                            hNotifyPipe, NULL) ? 
                            TRUE : 
                            (ERROR_PIPE_CONNECTED == GetLastError()); 

        if (FALSE == bConnected)
        {
            break;
        }

		//emit xpsPipeConnected();

		bool bErrorFound = false;

		INT nItem = 0;

		do
		{
			NOTIFYSTATE NotifyState; 

			//Reset the the structure.
			ZeroMemory(&NotifyState, sizeof(NotifyState));

			DWORD dwBytesRead = 0;

			DWORD dwBytesToRead = sizeof(NOTIFYSTATE);

			if (FALSE == ReadFile(hNotifyPipe,
								  //buffer to read from.
								  &NotifyState,
								  //number of bytes to read.
								  dwBytesToRead, 
								  //number of bytes read.
								  &dwBytesRead,   
								  //not overlapped I/O.
								  NULL))        
			{
				break;
			}

			//Compare the bytes read.
			if (dwBytesRead != dwBytesToRead)
			{
				break;
			}

			FDSNOTIFY FDSNotify = {0};

			FPNOTIFY FPNotify = {0};

			FDNOTIFY FDNotify = {0};

			//If the event is error then return failure.
			switch (NotifyState.eEvent)
			{
			case eFDS:
				{
					dwBytesRead = 0;

					dwBytesToRead = NotifyState.unSize;

					if (FALSE == ReadFile(hNotifyPipe,
										  //buffer to read from.
										  &FDSNotify,
										  //number of bytes to read.
										  dwBytesToRead, 
										  //number of bytes read.
										  &dwBytesRead,   
										  //not overlapped I/O.
										  NULL))        
					{
						bErrorFound = true;
						break;
					}

					//Compare the bytes read.
					if (dwBytesRead != dwBytesToRead)
					{
						bErrorFound = true;
						break;
					}
				}
				break;
			case eFD:
				{
					dwBytesRead = 0;

					dwBytesToRead = NotifyState.unSize;

					if (FALSE == ReadFile(hNotifyPipe,
										  //buffer to read from.
										  &FDNotify,
										  //number of bytes to read.
										  dwBytesToRead, 
										  //number of bytes read.
										  &dwBytesRead,   
										  //not overlapped I/O.
										  NULL))        
					{
						bErrorFound = true;
						break;
					}
					//Compare the bytes read.
					if (dwBytesRead != dwBytesToRead)
					{
						bErrorFound = true;
						break;
					}
				}
				break;
			case eFP:
				{
					/*if (sizeof(FPNOTIFY) != NotifyState.unSize)
					{
						bErrorFound = true;
						break;
					}*/

					dwBytesRead = 0;

					dwBytesToRead = NotifyState.unSize;

					if (FALSE == ReadFile(hNotifyPipe,
										  //buffer to read from.
										  &FPNotify,
										  //number of bytes to read.
										  dwBytesToRead, 
										  //number of bytes read.
										  &dwBytesRead,   
										  //not overlapped I/O.
										  NULL))        
					{
						bErrorFound = true;
						break;
					}
					//Compare the bytes read.
					if (dwBytesRead != dwBytesToRead)
					{
						bErrorFound = true;
						break;
					}

					addItem(QString::fromWCharArray(FPNotify.wszPageURI));
				}
				break;
			case eError:
				{
					QCoreApplication::quit();
				}
				break;
			default:
				{
					bErrorFound = true;
					break;
				}
			}
		}while(1);

		if (true == bErrorFound)
		{
			break;
		}

	} while (0);

	//Close pipe
	if (INVALID_HANDLE_VALUE != hNotifyPipe)
	{
		CloseHandle(hNotifyPipe);
		hNotifyPipe = INVALID_HANDLE_VALUE;
	}

	//Free the memory allocated for the SECURITY_DESCRIPTOR.
	if (NULL != SecurityAttributes.lpSecurityDescriptor)
	{
		if (NULL != LocalFree(SecurityAttributes.lpSecurityDescriptor))
		{
		}
	}
}