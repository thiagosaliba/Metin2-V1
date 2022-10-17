#if !defined(__INC_FILE_MONITOR_FREEBSD_H__) && !defined(__WIN32__)
#define __INC_FILE_MONITOR_FREEBSD_H__

#include "IFileMonitor.h" 
#include <unistd.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/time.h>

struct FileIOContext_FreeBSD
{
	int fhMonitor;
	int idxToEventList; // evtTrigger & evtMonitor index should be same
	PFN_FileChangeListener pListenFunc;
};

class FileMonitorFreeBSD : public IFileMonitor
{
private:
	FileMonitorFreeBSD(); // Hidden

public:
	virtual ~FileMonitorFreeBSD();

	void AddWatch(const std::string& strFileName, PFN_FileChangeListener pListenerFunc);
	void Update(DWORD dwPulses);

	static FileMonitorFreeBSD& Instance()
	{
		static FileMonitorFreeBSD theMonitor;
		return theMonitor;
	}

private:
	typedef boost::unordered_map<std::string, FileIOContext_FreeBSD> TMonitorFileHashMap;
	typedef std::vector<struct kevent> TEventList;

	TMonitorFileHashMap m_FileLists;
	TEventList m_MonitoredEventLists;
	TEventList m_TriggeredEventLists;

	int m_KernelEventQueue;
};

#endif // __INC_FILE_MONITOR_FREEBSD_H__
