#ifndef DOCCONVERTER_H
#define DOCCONVERTER_H
#include <Windows.h>
#include <QThread>
#include <QImage>

//Notify Event.
enum ENotifyEvent
{
	eError  = 0,
	eFDS    = 0x11001011,
	eFD     = 0x11001101,
	eFP     = 0x11001110
};

typedef struct _NOTIFYSTATE_
{
	//Notify Event.
	ENotifyEvent eEvent;
	//Data Size.
	UINT32 unSize;
}NOTIFYSTATE;


typedef struct _FPRESOURCES_
{
	//Count of Fonts.
	UINT32 unFonts;
	//Count of Images.
	UINT32 unImages;
	//Count of Color Context.
	UINT32 unColorContext;
	//Count of Resource Dictionary.
	UINT32 unResourceDictionary;
	//Count of Others.
	UINT32 unOthers;
}FPRESOURCES;

typedef struct _FPNOTIFY_
{   
	WCHAR wszPageURI[MAX_PATH]; 

	WCHAR wszPTURI[MAX_PATH];  
	//FP Resources.
	FPRESOURCES FPResources;
}FPNOTIFY;

typedef struct _FDSNOTIFY_
{
	 WCHAR wszPTURI[MAX_PATH]; 
}FDSNOTIFY;

typedef struct _FDNOTIFY_
{
	 WCHAR wszPTURI[MAX_PATH];  
}FDNOTIFY;

//Get Print Execution Data Function Pointer.
typedef bool FnConvertFPage2Bitmap(IN CONST CHAR* pszContainerPath,
								   IN CONST CHAR* pszFPage,
								   IN CONST CHAR* pszFImage,
								   IN INT nXRes,
								   IN INT nYRes);

class DocConverterThread : public QThread
{
	Q_OBJECT

public:
	DocConverterThread(QObject *parent);
	~DocConverterThread();

	void run();

signals:
	void addItem(QString);

private slots:

private:
};

#endif // DOCCONVERTER_H
