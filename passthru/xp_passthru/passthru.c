/*++

Copyright (c) 1992-2000  Microsoft Corporation
 
Module Name:
 
	passthru.c

Abstract:

	Ndis Intermediate Miniport driver sample. This is a passthru driver.

Author:

Environment:


Revision History:


--*/


#include "precomp.h"
#include "DDKLBLInc.h"
#include "fw_rule.h"
//#include "in_systm.h"
#include "DDKLBLInc.h"
#include "cons_def.h"
#include "fw_log.h"
#include "fw_interface.h"
#include "Rule.h"
#pragma hdrstop

#pragma NDIS_INIT_FUNCTION(DriverEntry)

NDIS_HANDLE						ProtHandle = NULL;
NDIS_HANDLE						DriverHandle = NULL;
NDIS_MEDIUM						MediumArray[4] =
									{
										NdisMedium802_3,	// Ethernet
										NdisMedium802_5,	// Token-ring
										NdisMediumFddi,		// Fddi
										NdisMediumWan       // NDISWAN
									};

NDIS_SPIN_LOCK	GlobalLock;

PADAPT			pAdaptList = NULL;
LONG            MiniportCount = 0;

NDIS_HANDLE		NdisWrapperHandle;

//
// To support ioctls from user-mode:
// Add by LornWolf
static NTSTATUS MydrvDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp); 
static NTSTATUS MydrvDispatchIoctl(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp); 

NDIS_HANDLE NdisDeviceHandle;		// ��ΪҪ�� MPHalt �����ã������ó�ȫ�֡�
PVOID gpEventObject = NULL;			// ��Ӧ�ó���ͨ�ŵ� Event ����
//UINT Monitor_flag;			// ���ӱ�־��1->���ӣ�0->�����ӡ�
//UINT Allow_ICMP;				// �Ƿ�����ICMP
//UINT Allow_TCP;				// �Ƿ�����TCP
//UINT Allow_UDP;				// �Ƿ�����UDP

struct Fw_Rules g_Rules;
struct Statistics  g_stat;
//struct All_Rules All_Rule;
struct ip 		g_IPHeader, *g_pIPHeader=NULL;
struct tcphdr	g_TCPHeader, *g_pTCPHeader=NULL;
struct udphdr	g_UDPHeader, *g_pUDPHeader=NULL;
struct icmp		g_ICMPHeader, *g_pICMPHeader=NULL;

struct Log g_Log, *g_pLog=NULL;//��ʱ�ӵġ������޸�

//struct All_Alarms 	g_AllAlarm[MAX_CALL_NUM];
//PFILE_OBJECT 		g_alarmfop=NULL;
//PDEVICE_EXTENSION 	g_alarmpdx=NULL;

//struct All_Logs		g_AllLog[MAX_CALL_NUM];
//PFILE_OBJECT 		g_logfop=NULL;
//PDEVICE_EXTENSION 	g_logpdx=NULL;

//FAST_MUTEX  fw_mutex;

extern USHORT	lcall_num,acall_num;

// ��Ӧ�ó������ڴ�
PVOID SystemVirtualAddress, UserVirtualAddress;
PMDL Mdl;
// End
//

#define LINKNAME_STRING     L"\\DosDevices\\MyPassthru"
#define NTDEVICE_STRING     L"\\Device\\MyPassthru"

NDIS_HANDLE     NdisDeviceHandle = NULL;
PDEVICE_OBJECT  ControlDeviceObject = NULL;

enum _DEVICE_STATE
{
	PS_DEVICE_STATE_READY = 0,	// ready for create/delete
	PS_DEVICE_STATE_CREATING,	// create operation in progress
	PS_DEVICE_STATE_DELETING	// delete operation in progress
}				ControlDeviceState = PS_DEVICE_STATE_READY;



NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
	)
    /*------------------------------------------------------------------------------
	��һ����protocolChar�ṹ�е�ÿһ������Щ��һЩ����������������İ汾�š����ֵ���Ϣ��
	����һЩ�Ǻ���ָ�룬����ָ����һЩNDIS�Զ�ִ�еĺ�������ڵ�ַ��
	����ָ����ÿһ�������ڳ����б��������ľ���ʵ�֡�������Щ����ָ�����ΪNULL��
	��ʾ��ָ��������������ǣ�NDISҪ��һЩ�����Ǳ���ָ���ģ����ǰ�����
	BindAdapterHandler��UnbindAdapterHandler��OpenAdapterCompleteHandler ��
	CloseAdapterCompleteHandler��ReceiveHandler ��ReceiveCompleteHandler��
	TransferCompleteHandler��SendCompleteHandler ��ResetCompleteHandler ��
	 RequestCompleteHandler StatusHandler ��StatusCompleteHandler ��PnPEventHandler 
	------------------------------------------------------------------------------*/
/*++

Routine Description:

	First entry point to be called, when this driver is loaded.
	Register with NDIS as an intermediate driver.

Arguments:

	DriverObject - pointer to the system's driver object structure
		for this driver
	
	RegistryPath - system's registry path for this driver
	
Return Value:

	STATUS_SUCCESS if all initialization is successful, STATUS_XXX
	error code if not.

--*/
{
	NDIS_STATUS						Status;
	NDIS_PROTOCOL_CHARACTERISTICS	PChars;//Э�鲿�ֻص�����
	NDIS_MINIPORT_CHARACTERISTICS	MChars;//С�˿���ػص�����
	PNDIS_CONFIGURATION_PARAMETER	Param;
	NDIS_STRING						Name;
	NDIS_HANDLE						WrapperHandle;

	// add by LornWolf
	UNICODE_STRING nameString, linkString; 
	UINT FuncIndex;
	PDEVICE_OBJECT MyDeviceObject; 
	PDRIVER_DISPATCH MajorFunction[IRP_MJ_MAXIMUM_FUNCTION + 1];
	//end add

	Status = NDIS_STATUS_SUCCESS;
	NdisAllocateSpinLock(&GlobalLock);

	NdisMInitializeWrapper(&NdisWrapperHandle, DriverObject, RegistryPath, NULL);

	do//����dowhilefalseΪ�˳���ֱ���˳���
	{
		//
		// Register the miniport with NDIS. Note that it is the miniport
		// which was started as a driver and not the protocol. Also the miniport
		// must be registered prior to the protocol since the protocol's BindAdapter
		// handler can be initiated anytime and when it is, it must be ready to
		// start driver instances.
		//
		NdisMInitializeWrapper(&WrapperHandle, DriverObject, RegistryPath, NULL);
		NdisZeroMemory(&MChars, sizeof(NDIS_MINIPORT_CHARACTERISTICS));

		MChars.MajorNdisVersion = PASSTHRU_MAJOR_NDIS_VERSION;
		MChars.MinorNdisVersion = PASSTHRU_MINOR_NDIS_VERSION;

		MChars.InitializeHandler = MPInitialize;
		MChars.QueryInformationHandler = MPQueryInformation;
		MChars.SetInformationHandler = MPSetInformation;
		MChars.ResetHandler = MPReset;
		MChars.TransferDataHandler = MPTransferData;
		MChars.HaltHandler = MPHalt;
#ifdef NDIS51_MINIPORT
		MChars.CancelSendPacketsHandler = MPCancelSendPackets;
		MChars.PnPEventNotifyHandler = MPDevicePnPEvent;
		MChars.AdapterShutdownHandler = MPAdapterShutdown;
#endif // NDIS51_MINIPORT

		//
		// We will disable the check for hang timeout so we do not
		// need a check for hang handler!
		//
		MChars.CheckForHangHandler = NULL;
		MChars.ReturnPacketHandler = MPReturnPacket;//��ʱ���Ѿ�û��IRP��������ˣ�����ֱ����
		//���������ˡ���tdi��ά���˿ں�ip��ַ��һ������ndis���õ��˿�ȥ���Ǹ���
		//
		// Either the Send or the SendPackets handler should be specified.
		// If SendPackets handler is specified, SendHandler is ignored
		//�������������ط����ĺ�����
		MChars.SendHandler = NULL;	// MPSend;
		MChars.SendPacketsHandler = MPSendPackets;

		Status = NdisIMRegisterLayeredMiniport(NdisWrapperHandle,
   											   &MChars,
   											   sizeof(MChars),
										   	   &DriverHandle);
		if (Status != NDIS_STATUS_SUCCESS)
		{
			break;
		}

#ifndef WIN9X
		NdisMRegisterUnloadHandler(NdisWrapperHandle, PtUnload);
#endif

		//
		// Now register the protocol.
		//
		NdisZeroMemory(&PChars, sizeof(NDIS_PROTOCOL_CHARACTERISTICS));
		PChars.MajorNdisVersion = PASSTHRU_PROT_MAJOR_NDIS_VERSION;
		PChars.MinorNdisVersion = PASSTHRU_PROT_MINOR_NDIS_VERSION;

		//
		// Make sure the protocol-name matches the service-name
		// (from the INF) under which this protocol is installed.
		// This is needed to ensure that NDIS can correctly determine
		// the binding and call us to bind to miniports below.
		//
		NdisInitUnicodeString(&Name, L"Passthru");	// Protocol name
		PChars.Name = Name;
		PChars.OpenAdapterCompleteHandler = PtOpenAdapterComplete;
		PChars.CloseAdapterCompleteHandler = PtCloseAdapterComplete;
		PChars.SendCompleteHandler = PtSendComplete;
		PChars.TransferDataCompleteHandler = PtTransferDataComplete;
	
		PChars.ResetCompleteHandler = PtResetComplete;
		PChars.RequestCompleteHandler = PtRequestComplete;
		PChars.ReceiveHandler = PtReceive;
		PChars.ReceiveCompleteHandler = PtReceiveComplete;
		PChars.StatusHandler = PtStatus;
		PChars.StatusCompleteHandler = PtStatusComplete;
		PChars.BindAdapterHandler = PtBindAdapter;
		PChars.UnbindAdapterHandler = PtUnbindAdapter;
		PChars.UnloadHandler = PtUnloadProtocol;

		PChars.ReceivePacketHandler = PtReceivePacket;
		PChars.PnPEventHandler= PtPNPHandler;

		NdisRegisterProtocol(&Status,
 							&ProtHandle,
 							&PChars,
 							sizeof(NDIS_PROTOCOL_CHARACTERISTICS));

		if (Status != NDIS_STATUS_SUCCESS)
		{
			NdisIMDeregisterLayeredMiniport(DriverHandle);
			break;
		}

		NdisIMAssociateMiniport(DriverHandle, ProtHandle);
	}
	while (FALSE);

//----------------- �����豸�������������----------------------------add by xiaoqiang
	RtlInitUnicodeString(&nameString, L"\\Device\\MyPassthru" ); 
	RtlInitUnicodeString(&linkString, L"\\??\\MyPassthru"); 

	for(FuncIndex = 0; FuncIndex <=IRP_MJ_MAXIMUM_FUNCTION; FuncIndex++)
	{
		MajorFunction[FuncIndex] = NULL;
	}
	    
	MajorFunction[IRP_MJ_CREATE]         = MydrvDispatch;
	MajorFunction[IRP_MJ_CLOSE]          = MydrvDispatch;
	MajorFunction[IRP_MJ_DEVICE_CONTROL] = MydrvDispatchIoctl;//�·�����

	Status = NdisMRegisterDevice(
                    WrapperHandle, 
                    &nameString,
                    &linkString,
                    MajorFunction,
                    &MyDeviceObject,
                    &NdisDeviceHandle
                    );

	if(Status != STATUS_SUCCESS)
	{
		DbgPrint("NdisMRegisterDevice failed!\n");
	}

    // ��ʼ��config����
	DbgPrint("��ʼ��config����!\n");
	g_Rules.global.fw_status=FWS_ALLOWALL;//����
	g_Rules.global.log_disable=0;
	g_Rules.global.fw_mode=RM_ALLOW;
	g_Rules.tcp.disable=0;
	g_Rules.udp.disable=0;
	g_Rules.icmp.disable=0;



	// ������Ӧ�ó�������ڴ�
	SystemVirtualAddress = ExAllocatePool(NonPagedPool, 1024);
	Mdl = IoAllocateMdl(SystemVirtualAddress, 1024, FALSE, FALSE, NULL);
	MmBuildMdlForNonPagedPool(Mdl);


	if (Status != NDIS_STATUS_SUCCESS)
	{
		NdisTerminateWrapper(NdisWrapperHandle, NULL);
	}

	g_pIPHeader=&g_IPHeader;
	g_pTCPHeader=&g_TCPHeader;
	g_pUDPHeader=&g_UDPHeader;
	g_pICMPHeader=&g_ICMPHeader;
	g_pLog = &g_Log;

	return(Status);
}

//-------------------------------------------------------------add by xiaoqiang
static NTSTATUS MydrvDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp) 
{ 
	NTSTATUS status; 
	PIO_STACK_LOCATION irpSp; 

//	UNREFERENCED_PARAMETER(DeviceObject); 
	irpSp = IoGetCurrentIrpStackLocation(pIrp); 

	switch (irpSp->MajorFunction) 
	{ 
		case IRP_MJ_CREATE: 
			pIrp->IoStatus.Status = STATUS_SUCCESS; 
			pIrp->IoStatus.Information = 0L; 
			break; 

		case IRP_MJ_CLOSE: 
			pIrp->IoStatus.Status = STATUS_SUCCESS; 
			pIrp->IoStatus.Information = 0L; 
			MmUnmapLockedPages(UserVirtualAddress, Mdl);
			break; 
	} 

	IoCompleteRequest(pIrp, IO_NO_INCREMENT); 
	return STATUS_SUCCESS; 

} 

static NTSTATUS MydrvDispatchIoctl(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp) 
{ 


	OBJECT_HANDLE_INFORMATION	objHandleInfo;
	HANDLE	hEvent = NULL;
	//--------------------------------------------------------------
	NTSTATUS status = STATUS_SUCCESS;

	// ȡ�ô�IRP��pIrp����I/O��ջָ��
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// ȡ��I/O���ƴ���
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// ȡ��I/O������ָ������ĳ���
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	//--------------------------------------------------------------

	switch (uIoControlCode) 
	{ 
		case IO_GET_SHAREMEMORY_ADDR:
			// ��������ڴ�ӳ�䵽�û����̵�ַ�ռ䣬�����ص�ַ��
			try
			{
				UserVirtualAddress = MmMapLockedPages(Mdl, UserMode); 
				*((PVOID *)pIoBuffer) = UserVirtualAddress;
				pIrp->IoStatus.Status = STATUS_SUCCESS; 
				pIrp->IoStatus.Information = sizeof(PVOID);
			}
			except(EXCEPTION_EXECUTE_HANDLER){}
			break;

		case IO_REFERENCE_EVENT: 
			hEvent = (HANDLE)pIrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
			status = ObReferenceObjectByHandle(hEvent,
												GENERIC_ALL,
												NULL,
												KernelMode,
												&gpEventObject,
												&objHandleInfo);
			if(status != STATUS_SUCCESS)
			{
				DbgPrint("ObReferenceObjectByHandle failed! status = %x\n", status);
				break;
			}
			else 
			{
				memset(&g_stat, 0, sizeof(struct Statistics));//Ӧ�ý����ؿ���Ҫ��ʼ��g_stat����0��
				DbgPrint("Referenct object sussfully!\n");
			}
			pIrp->IoStatus.Status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0L;
			break;

		case IO_STOP_MONITOR_EVENT:
			UTIL_PASSALL();    // ���ü���ģʽΪPASSALL
			UTIL_DelAllrules();

			if(gpEventObject)
			{
				ObDereferenceObject(gpEventObject);
				DbgPrint("ObDereferenceObject sussfully!\n");
			}
			else{}
			pIrp->IoStatus.Status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0L;
			break;

		case IO_PASS_ALL:
	        UTIL_PASSALL();    // ���ü���ģʽΪPASSALL

			pIrp->IoStatus.Status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0L;
			break;

		case IO_DENY_ALL:
	        UTIL_DENYALL();    // ���ü���ģʽΪDENYALL

			pIrp->IoStatus.Status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0L;
			break;

		case IO_CLEAR_EVENT:
			KeClearEvent(gpEventObject);
			DbgPrint("KeClearEvent sussfully!\n");
			pIrp->IoStatus.Status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0L;
			break;

		case IO_ADD_RULE:	// ���һ�����˹���
			DbgPrint("==>AddOneRule\n");
			DbgPrint("uInSize=%d,sizeof(struct Rules)=%d",uInSize,sizeof(struct Rules));
			if(uInSize == sizeof(struct Rules))
			{
				if(UTIL_AddOneRule((struct Rules*)pIoBuffer))
					pIrp->IoStatus.Status = STATUS_SUCCESS;
				DbgPrint("<==AddOneRule\n");
			}
			else 
				pIrp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
			pIrp->IoStatus.Information = 0L;
			break;

		case IO_DEL_RULE:
			DbgPrint("==>DelOneRule\n");
			DbgPrint("uInSize=%d,sizeof(struct Rules)=%d",uInSize,sizeof(struct Rules));
			if(uInSize == sizeof(struct Rules))
			{
				if(UTIL_DelOneRule((struct Rules*)pIoBuffer))
					pIrp->IoStatus.Status = STATUS_SUCCESS;
				DbgPrint("<==DelOneRule\n");
			}
			else 
				pIrp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
			pIrp->IoStatus.Information = 0L;
			break;

		case IO_CLEAR_RULES:
			DbgPrint("==>ClearAllRule\n");
			if(UTIL_DelAllrules())
				pIrp->IoStatus.Status = STATUS_SUCCESS;
			else 
				pIrp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
			DbgPrint("<==ClearAllRule\n");
			pIrp->IoStatus.Information = 0L;
			break;

		case IO_ADD_CONFIG:
			DbgPrint("==>AddConfig!\n");
			if(uInSize == sizeof(struct RuleSet))
			{
				if(UTIL_AddConfig((struct RuleSet*)pIoBuffer))
					pIrp->IoStatus.Status = STATUS_SUCCESS;
				DbgPrint("<==AddConfig!\n");
			}
			else 
				pIrp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
				pIrp->IoStatus.Information = 0L;
			break;

		case IO_GET_Stat:
			DbgPrint("==>GetStatistics!\n");
			// ����g_stat������Ӧ�ò㡣
			try
			{
				*((struct Statistics *)pIoBuffer)=g_stat;

	//�˴��и�BUG����Ϊ������>int��4B����ʱ������������2G�����ݰ�����������ʹ�÷������ʺ��٣�
	//���Ҫ�����������޶���С����0         !!!!!!!!!!!!!!  may 10 2006 Edit by xiaoqiang
				pIrp->IoStatus.Status = STATUS_SUCCESS; 
				pIrp->IoStatus.Information = sizeof(struct Statistics);
			}
			except(EXCEPTION_EXECUTE_HANDLER){}
			DbgPrint("<==GetStatistics!\n");
			break;


		default:
			break;
	}

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status; 
} 
//------------------------------------------------------------end add


NDIS_STATUS
PtRegisterDevice(
	VOID
	)
/*++

Routine Description:

	Register an ioctl interface - a device object to be used for this
	purpose is created by NDIS when we call NdisMRegisterDevice.

	This routine is called whenever a new miniport instance is
	initialized. However, we only create one global device object,
	when the first miniport instance is initialized. This routine
	handles potential race conditions with PtDeregisterDevice via
	the ControlDeviceState and MiniportCount variables.

	NOTE: do not call this from DriverEntry; it will prevent the driver
	from being unloaded (e.g. on uninstall).

Arguments:

	None

Return Value:

	NDIS_STATUS_SUCCESS if we successfully register a device object.

--*/
{
	NDIS_STATUS			Status = NDIS_STATUS_SUCCESS;
	UNICODE_STRING		DeviceName;
	UNICODE_STRING		DeviceLinkUnicodeString;
	PDRIVER_DISPATCH	DispatchTable[IRP_MJ_MAXIMUM_FUNCTION];
	UINT				i;

	DBGPRINT(("==>PtRegisterDevice\n"));

	NdisAcquireSpinLock(&GlobalLock);

	++MiniportCount;
	
	if (1 == MiniportCount)
    {
		ASSERT(ControlDeviceState != PS_DEVICE_STATE_CREATING);

		//
		// Another thread could be running PtDeregisterDevice on
		// behalf of another miniport instance. If so, wait for
		// it to exit.
		//
		while (ControlDeviceState != PS_DEVICE_STATE_READY)
		{
			NdisReleaseSpinLock(&GlobalLock);
			NdisMSleep(1);
			NdisAcquireSpinLock(&GlobalLock);
		}

		ControlDeviceState = PS_DEVICE_STATE_CREATING;

		NdisReleaseSpinLock(&GlobalLock);

		for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		{
		    DispatchTable[i] = PtDispatch;
		}

		NdisInitUnicodeString(&DeviceName, NTDEVICE_STRING);
		NdisInitUnicodeString(&DeviceLinkUnicodeString, LINKNAME_STRING);

		//
		// Create a device object and register our dispatch handlers
		//
		
		Status = NdisMRegisterDevice(
                    NdisWrapperHandle, 
                    &DeviceName,
                    &DeviceLinkUnicodeString,
                    &DispatchTable[0],
                    &ControlDeviceObject,
                    &NdisDeviceHandle
                    );

		NdisAcquireSpinLock(&GlobalLock);

		ControlDeviceState = PS_DEVICE_STATE_READY;
	}

	NdisReleaseSpinLock(&GlobalLock);

	DBGPRINT(("<==PtRegisterDevice: %x\n", Status));

	return (Status);
}


NTSTATUS
PtDispatch(
	IN PDEVICE_OBJECT	DeviceObject,
	IN PIRP				pIrp
	)
/*++
Routine Description:

	Process IRPs sent to this device.

Arguments:

	DeviceObject - pointer to a device object
	pIrp	  - pointer to an I/O Request Packet

Return Value:

	NTSTATUS - STATUS_SUCCESS always - change this when adding
	real code to handle ioctls.

--*/
{
	PIO_STACK_LOCATION  irpStack;
	NTSTATUS	        status = STATUS_SUCCESS;

	DBGPRINT(("==>Pt Dispatch\n"));
	irpStack = IoGetCurrentIrpStackLocation(pIrp);
	  
	switch (irpStack->MajorFunction)
	{
		case IRP_MJ_CREATE:
			break;
		case IRP_MJ_CLOSE:
			break;        
		case IRP_MJ_DEVICE_CONTROL:
			//
			// Add code here to handle ioctl commands sent to passthru.
			//
			break;        
		default:
			break;
	}

	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DBGPRINT(("<== Pt Dispatch\n"));

	return status;

} 


NDIS_STATUS
PtDeregisterDevice(
	VOID
	)
/*++

Routine Description:

	Deregister the ioctl interface. This is called whenever a miniport
	instance is halted. When the last miniport instance is halted, we
	request NDIS to delete the device object

Arguments:

	NdisDeviceHandle - Handle returned by NdisMRegisterDevice

Return Value:

	NDIS_STATUS_SUCCESS if everything worked ok

--*/
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

	DBGPRINT(("==>PassthruDeregisterDevice\n"));

	NdisAcquireSpinLock(&GlobalLock);

	ASSERT(MiniportCount > 0);

	--MiniportCount;
	
	if (0 == MiniportCount)
	{
		//
		// All miniport instances have been halted. Deregister
		// the control device.
		//

		ASSERT(ControlDeviceState == PS_DEVICE_STATE_READY);

		//
		// Block PtRegisterDevice() while we release the control
		// device lock and deregister the device.
		// 
		ControlDeviceState = PS_DEVICE_STATE_DELETING;

		NdisReleaseSpinLock(&GlobalLock);

		if (NdisDeviceHandle != NULL)
		{
			Status = NdisMDeregisterDevice(NdisDeviceHandle);
			NdisDeviceHandle = NULL;
		}

		NdisAcquireSpinLock(&GlobalLock);
		ControlDeviceState = PS_DEVICE_STATE_READY;
	}

	NdisReleaseSpinLock(&GlobalLock);

	DBGPRINT(("<== PassthruDeregisterDevice: %x\n", Status));
	return Status;
	
}


VOID
PtUnloadProtocol(
    VOID
)
{
	NDIS_STATUS Status;

	if (ProtHandle != NULL)
	{
		NdisDeregisterProtocol(&Status, ProtHandle);
		ProtHandle = NULL;
	}

	DBGPRINT(("PtUnloadProtocol: done!\n"));
}

