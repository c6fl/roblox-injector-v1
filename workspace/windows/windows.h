#pragma once

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	_Field_size_bytes_part_opt_( MaximumLength, Length ) PWCH Buffer;
}
UNICODE_STRING, * PUNICODE_STRING;
typedef struct _SYSTEM_MODULE
{
	PVOID Reserved1;
	PVOID Reserved2;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT Index;
	USHORT NameLength;
	USHORT LoadCount;
	USHORT PathLength;
	CHAR ImageName[ 256 ];
}
SYSTEM_MODULE, * PSYSTEM_MODULE;
typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG ModulesCount;
	SYSTEM_MODULE Modules[ 0 ];
}
SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;
typedef LONG KPRIORITY;
struct CLIENT_ID
{
	DWORD UniqueProcess;  ULONG pad1;
	DWORD UniqueThread;  ULONG pad2;
};
typedef struct
{
	FILETIME ProcessorTime;
	FILETIME UserTime;
	FILETIME CreateTime;
	ULONG WaitTime;
	ULONG pad1;
	PVOID StartAddress;
	CLIENT_ID Client_Id;
	KPRIORITY CurrentPriority;
	KPRIORITY BasePriority;
	ULONG ContextSwitchesPerSec;
	ULONG ThreadState;
	ULONG ThreadWaitReason;
	ULONG pad2;
}
SYSTEM_THREAD_INFORMATION;
typedef struct
{
	ULONG_PTR PeakVirtualSize;
	ULONG_PTR VirtualSize;
	ULONG PageFaultCount;
	ULONG pad1;
	ULONG_PTR PeakWorkingSetSize;
	ULONG_PTR WorkingSetSize;
	ULONG_PTR QuotaPeakPagedPoolUsage;
	ULONG_PTR QuotaPagedPoolUsage;
	ULONG_PTR QuotaPeakNonPagedPoolUsage;
	ULONG_PTR QuotaNonPagedPoolUsage;
	ULONG_PTR PagefileUsage;
	ULONG_PTR PeakPagefileUsage;
}
VM_COUNTERS;
typedef struct
{
	ULONG NextOffset;
	ULONG ThreadCount;
	LARGE_INTEGER WorkingSetPrivateSize;
	ULONG HardFaultCount;
	ULONG NumberOfThreadsHighWatermark;
	ULONGLONG CycleTime;
	FILETIME CreateTime;
	FILETIME UserTime;
	FILETIME KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	ULONG pad1;
	ULONG ProcessId;
	ULONG pad2;
	ULONG InheritedFromProcessId;
	ULONG pad3;
	ULONG HandleCount;
	ULONG SessionId;
	ULONG_PTR UniqueProcessKey;
	VM_COUNTERS VirtualMemoryCounters;
	ULONG_PTR PrivatePageCount;
	IO_COUNTERS IoCounters;
	SYSTEM_THREAD_INFORMATION ThreadInfos[ 1 ];
}
SYSTEM_PROCESS_INFORMATION;
typedef struct _PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION
{
	ULONG Version;
	ULONG Reserved;
	PVOID Callback;
}
PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION, * PPROCESS_INSTRUMENTATION_CALLBACK_INFORMATION;
typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemNextEventIdInformation,
	SystemEventIdsInformation,
	SystemCrashDumpInformation,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemPlugPlayBusInformation,
	SystemDockInformation
}
SYSTEM_INFORMATION_CLASS, * PSYSTEM_INFORMATION_CLASS;
typedef struct _PROCESS_HANDLE_TABLE_ENTRY_INFO
{
	HANDLE HandleValue;
	ULONG_PTR HandleCount;
	ULONG_PTR PointerCount;
	ULONG GrantedAccess;
	ULONG ObjectTypeIndex;
	ULONG HandleAttributes;
	ULONG Reserved;
}
PROCESS_HANDLE_TABLE_ENTRY_INFO, * PPROCESS_HANDLE_TABLE_ENTRY_INFO;
typedef enum _PROCESSINFOCLASS
{
	ProcessBasicInformation, ProcessQuotaLimits, ProcessIoCounters, ProcessVmCounters, ProcessTimes, ProcessBasePriority, ProcessRaisePriority, ProcessDebugPort, ProcessExceptionPort, ProcessAccessToken, ProcessLdtInformation, ProcessLdtSize, ProcessDefaultHardErrorMode, ProcessIoPortHandlers, ProcessPooledUsageAndLimits, ProcessWorkingSetWatch, ProcessUserModeIOPL, ProcessEnableAlignmentFaultFixup, ProcessPriorityClass, ProcessWx86Information, ProcessHandleCount, ProcessAffinityMask, ProcessPriorityBoost, ProcessDeviceMap, ProcessSessionInformation, ProcessForegroundInformation, ProcessWow64Information, ProcessImageFileName, ProcessLUIDDeviceMapsEnabled, ProcessBreakOnTermination, ProcessDebugObjectHandle, ProcessDebugFlags, ProcessHandleTracing, ProcessIoPriority, ProcessExecuteFlags, ProcessTlsInformation, ProcessCookie, ProcessImageInformation, ProcessCycleTime, ProcessPagePriority, ProcessInstrumentationCallback, ProcessThreadStackAllocation, ProcessWorkingSetWatchEx, ProcessImageFileNameWin32, ProcessImageFileMapping, ProcessAffinityUpdateMode, ProcessMemoryAllocationMode, ProcessGroupInformation, ProcessTokenVirtualizationEnabled, ProcessConsoleHostProcess, ProcessWindowInformation, ProcessHandleInformation, ProcessMitigationPolicy, ProcessDynamicFunctionTableInformation, ProcessHandleCheckingMode, ProcessKeepAliveCount, ProcessRevokeFileHandles, ProcessWorkingSetControl, ProcessHandleTable, ProcessCheckStackExtentsMode, ProcessCommandLineInformation, ProcessProtectionInformation, ProcessMemoryExhaustion, ProcessFaultInformation, ProcessTelemetryIdInformation, ProcessCommitReleaseInformation, ProcessDefaultCpuSetsInformation, ProcessAllowedCpuSetsInformation, ProcessSubsystemProcess,
	ProcessJobMemoryInformation, ProcessInPrivate, ProcessRaiseUMExceptionOnInvalidHandleClose, ProcessIumChallengeResponse,
	ProcessChildProcessInformation, ProcessHighGraphicsPriorityInformation, ProcessSubsystemInformation, ProcessEnergyValues, ProcessPowerThrottlingState, ProcessReserved3Information, ProcessWin32kSyscallFilterInformation, ProcessDisableSystemAllowedCpuSets, ProcessWakeInformation, ProcessEnergyTrackingState, ProcessManageWritesToExecutableMemory, ProcessCaptureTrustletLiveDump,
	ProcessTelemetryCoverage, ProcessEnclaveInformation,
	ProcessEnableReadWriteVmLogging, ProcessUptimeInformation, ProcessImageSection, ProcessDebugAuthInformation, ProcessSystemResourceManagement, ProcessSequenceNumber, ProcessLoaderDetour, ProcessSecurityDomainInformation, ProcessCombineSecurityDomainsInformation, ProcessEnableLogging, ProcessLeapSecondInformation, ProcessFiberShadowStackAllocation, ProcessFreeFiberShadowStackAllocation, ProcessAltSystemCallInformation, ProcessDynamicEHContinuationTargets, ProcessDynamicEnforcedCetCompatibleRanges, ProcessCreateStateChange, ProcessApplyStateChange,
	ProcessEnableOptionalXStateFeatures, ProcessAltPrefetchParam, ProcessAssignCpuPartitions,
	ProcessPriorityClassEx, ProcessMembershipInformation, ProcessEffectiveIoPriority, ProcessEffectivePagePriority, MaxProcessInfoClass
}
PROCESSINFOCLASS;
typedef enum _THREADINFOCLASS
{
	ThreadBasicInformation, ThreadTimes, ThreadPriority, ThreadBasePriority, ThreadAffinityMask, ThreadImpersonationToken, ThreadDescriptorTableEntry, ThreadEnableAlignmentFaultFixup, ThreadEventPair,
	ThreadQuerySetWin32StartAddress, ThreadZeroTlsCell, ThreadPerformanceCount, ThreadAmILastThread, ThreadIdealProcessor, ThreadPriorityBoost, ThreadSetTlsArrayAddress, ThreadIsIoPending, ThreadHideFromDebugger, ThreadBreakOnTermination, ThreadSwitchLegacyState, ThreadIsTerminated, ThreadLastSystemCall, ThreadIoPriority, ThreadCycleTime, ThreadPagePriority, ThreadActualBasePriority, ThreadTebInformation, ThreadCSwitchMon, ThreadCSwitchPmu,
	ThreadWow64Context, ThreadGroupInformation, ThreadUmsInformation, ThreadCounterProfiling, ThreadIdealProcessorEx, ThreadCpuAccountingInformation, ThreadSuspendCount, ThreadHeterogeneousCpuPolicy, ThreadContainerId, ThreadNameInformation, ThreadSelectedCpuSets,
	ThreadSystemThreadInformation, ThreadActualGroupAffinity, ThreadDynamicCodePolicyInfo, ThreadExplicitCaseSensitivity, ThreadWorkOnBehalfTicket, ThreadSubsystemInformation, ThreadDbgkWerReportActive, ThreadAttachContainer, ThreadManageWritesToExecutableMemory, ThreadPowerThrottlingState, ThreadWorkloadClass, ThreadCreateStateChange, ThreadApplyStateChange,
	ThreadStrongerBadHandleChecks, ThreadEffectiveIoPriority, ThreadEffectivePagePriority, ThreadUpdateLockOwnership, ThreadSchedulerSharedDataSlot,
	ThreadTebInformationAtomic, ThreadIndexInformation, MaxThreadInfoClass
}
THREADINFOCLASS;
typedef struct _OBJECT_TYPE_INFORMATION
{
	UNICODE_STRING TypeName;
	ULONG TotalNumberOfObjects;
	ULONG TotalNumberOfHandles;
	ULONG TotalPagedPoolUsage;
	ULONG TotalNonPagedPoolUsage;
	ULONG TotalNamePoolUsage;
	ULONG TotalHandleTableUsage;
	ULONG HighWaterNumberOfObjects;
	ULONG HighWaterNumberOfHandles;
	ULONG HighWaterPagedPoolUsage;
	ULONG HighWaterNonPagedPoolUsage;
	ULONG HighWaterNamePoolUsage;
	ULONG HighWaterHandleTableUsage;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccessMask;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	UCHAR TypeIndex;  CHAR ReservedByte;
	ULONG PoolType;
	ULONG DefaultPagedPoolCharge;
	ULONG DefaultNonPagedPoolCharge;
}
OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;
typedef struct _PROCESS_HANDLE_SNAPSHOT_INFORMATION
{
	ULONG_PTR NumberOfHandles;
	ULONG_PTR Reserved;
	_Field_size_( NumberOfHandles ) PROCESS_HANDLE_TABLE_ENTRY_INFO Handles[ 1 ];
}
PROCESS_HANDLE_SNAPSHOT_INFORMATION, * PPROCESS_HANDLE_SNAPSHOT_INFORMATION;
typedef enum _MEMORY_INFORMATION_CLASS
{
	MemoryBasicInformation, MemoryWorkingSetInformation, MemoryMappedFilenameInformation, MemoryRegionInformation, MemoryWorkingSetExInformation, MemorySharedCommitInformation, MemoryImageInformation, MemoryRegionInformationEx, MemoryPrivilegedBasicInformation, MemoryEnclaveImageInformation, MemoryBasicInformationCapped, MemoryPhysicalContiguityInformation, MemoryBadInformation, MemoryBadInformationAllProcesses, MaxMemoryInfoClass
}
MEMORY_INFORMATION_CLASS;
typedef struct _MEMORY_WORKING_SET_EX_BLOCK
{
	union
	{
		struct
		{
			ULONG_PTR Valid : 1;
			ULONG_PTR ShareCount : 3;
			ULONG_PTR Win32Protection : 11;
			ULONG_PTR Shared : 1;
			ULONG_PTR Node : 6;
			ULONG_PTR Locked : 1;
			ULONG_PTR LargePage : 1;
			ULONG_PTR Priority : 3;
			ULONG_PTR Reserved : 3;
			ULONG_PTR SharedOriginal : 1;
			ULONG_PTR Bad : 1;
			ULONG_PTR Win32GraphicsProtection : 4;  ULONG_PTR ReservedUlong : 28;
		};
		struct
		{
			ULONG_PTR Valid : 1;
			ULONG_PTR Reserved0 : 14;
			ULONG_PTR Shared : 1;
			ULONG_PTR Reserved1 : 5;
			ULONG_PTR PageTable : 1;
			ULONG_PTR Location : 2;
			ULONG_PTR Priority : 3;
			ULONG_PTR ModifiedList : 1;
			ULONG_PTR Reserved2 : 2;
			ULONG_PTR SharedOriginal : 1;
			ULONG_PTR Bad : 1;
			ULONG_PTR ReservedUlong : 32;
		}
		Invalid;
	};
}
MEMORY_WORKING_SET_EX_BLOCK, * PMEMORY_WORKING_SET_EX_BLOCK;
typedef struct _MEMORY_WORKING_SET_EX_INFORMATION
{
	PVOID VirtualAddress;
	union
	{
		MEMORY_WORKING_SET_EX_BLOCK VirtualAttributes;
		ULONG_PTR Long;
	}
	u1;
}
MEMORY_WORKING_SET_EX_INFORMATION, * PMEMORY_WORKING_SET_EX_INFORMATION;
typedef struct _MEMORY_WORKING_SET_BLOCK
{
	ULONG_PTR Protection : 5;
	ULONG_PTR ShareCount : 3;
	ULONG_PTR Shared : 1;
	ULONG_PTR Node : 3;
	ULONG_PTR VirtualPage : 52;
}
MEMORY_WORKING_SET_BLOCK, * PMEMORY_WORKING_SET_BLOCK;
typedef struct _MEMORY_WORKING_SET_INFORMATION
{
	ULONG_PTR NumberOfEntries;
	_Field_size_( NumberOfEntries ) MEMORY_WORKING_SET_BLOCK WorkingSetInfo[ 1 ];
}
MEMORY_WORKING_SET_INFORMATION, * PMEMORY_WORKING_SET_INFORMATION;
typedef struct _MEMORY_REGION_INFORMATION
{
	PVOID AllocationBase;
	ULONG AllocationProtect;
	union
	{
		ULONG RegionType;
		struct
		{
			ULONG Private : 1;
			ULONG MappedDataFile : 1;
			ULONG MappedImage : 1;
			ULONG MappedPageFile : 1;
			ULONG MappedPhysical : 1;
			ULONG DirectMapped : 1;
			ULONG SoftwareEnclave : 1;  ULONG PageSize64K : 1;
			ULONG PlaceholderReservation : 1;  ULONG MappedAwe : 1;  ULONG MappedWriteWatch : 1;
			ULONG PageSizeLarge : 1;
			ULONG PageSizeHuge : 1;
			ULONG Reserved : 19;
		};
	};
	SIZE_T RegionSize;
	SIZE_T CommitSize;
	ULONG_PTR PartitionId;  ULONG_PTR NodePreference;
}
MEMORY_REGION_INFORMATION, * PMEMORY_REGION_INFORMATION;
typedef struct _MEMORY_SHARED_COMMIT_INFORMATION
{
	SIZE_T CommitSize;
}
MEMORY_SHARED_COMMIT_INFORMATION, * PMEMORY_SHARED_COMMIT_INFORMATION;
typedef struct _MEMORY_IMAGE_INFORMATION
{
	PVOID ImageBase;
	SIZE_T SizeOfImage;
	union
	{
		ULONG ImageFlags;
		struct
		{
			ULONG ImagePartialMap : 1;
			ULONG ImageNotExecutable : 1;
			ULONG ImageSigningLevel : 4;  ULONG ImageExtensionPresent : 1;  ULONG Reserved : 25;
		};
	};
}
MEMORY_IMAGE_INFORMATION, * PMEMORY_IMAGE_INFORMATION;
typedef NTSTATUS( *NtSetInformationProcess_t )( HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength );
typedef NTSTATUS( *NtQueryVirtualMemory_t )( HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength );
using OurRtlRestoreContext_ty = void ( * )( PCONTEXT ContextRecord, _EXCEPTION_RECORD* ExceptionRecord );
using NtProtectVirtualMemory_t = NTSTATUS( * )( _In_ HANDLE ProcessHandle, _Inout_ PVOID* BaseAddress, _Inout_ PSIZE_T RegionSize, _In_ ULONG NewProtect, _Out_ PULONG OldProtect );
#pragma once

#pragma once

#include <Windows.h>
#include <cstdint>

typedef struct _APC_TASK_CALLBACKS
{
	void* ExecuteCallback;
	void* Unposted;
} APC_TASK_CALLBACKS, * PAPC_TASK_CALLBACKS;

typedef struct _APC_TASK
{
	struct _APC_TASK_CALLBACKS* Callbacks;
	UINT32 NumaNode;
	UINT8 IdealProcessor;
	char Padding_242[ 3 ];
	struct _LIST_ENTRY ListEntry;
} APC_TASK, * PAPC_TASK;

typedef struct _APCP_REFCOUNT
{
	volatile INT32 Refcount;
} APCP_REFCOUNT, * PAPCP_REFCOUNT;

typedef struct _APCP_CALLER
{
	void* ReturnAddress;
} APCP_CALLER, * PAPCP_CALLER;

typedef struct _APCP_PH
{
	struct _APCP_PH_LINKS* Root;
} APCP_PH, * PAPCP_PH;

typedef struct _APC_DIRECT
{
	struct _APC_TASK Task;
	UINT64 Lock;
	struct _LIST_ENTRY IoCompletionInformationList;
	void* Callback;
	UINT32 NumaNode;
	UINT8 IdealProcessor;
	char __PADDING__[ 3 ];
} APC_DIRECT, * PAPC_DIRECT;

typedef struct _APCP_TIMER_SUBQUEUE
{
	INT64 Expiration;
	struct _APCP_PH WindowStart;
	struct _APCP_PH WindowEnd;
	void* Timer;
	void* TimerPkt;
	struct _APC_DIRECT Direct;
	UINT32 ExpirationWindow;
	INT32 __PADDING__[ 1 ];
} APCP_TIMER_SUBQUEUE, * PAPCP_TIMER_SUBQUEUE;

typedef struct _APCP_TIMER_QUEUE
{
	struct _RTL_SRWLOCK Lock;
	struct _APCP_TIMER_SUBQUEUE AbsoluteQueue;
	struct _APCP_TIMER_SUBQUEUE RelativeQueue;
	INT32 AllocatedTimerCount;
	INT32 __PADDING__[ 1 ];
} APCP_TIMER_QUEUE, * PAPCP_TIMER_QUEUE;

typedef struct _APCP_NUMA_NODE
{
	INT32 WorkerCount;
} APCP_NUMA_NODE, * PAPCP_NUMA_NODE;

typedef union _APCP_POOL_QUEUE_STATE
{
	union
	{
		INT64 Exchange;
		struct
		{
			INT32 RunningThreadGoal : 16;
			UINT32 PendingReleaseCount : 16;
			UINT32 QueueLength;
		};
	};
} APCP_POOL_QUEUE_STATE, * PAPCP_POOL_QUEUE_STATE;

typedef struct _APCP_QUEUE
{
	struct _LIST_ENTRY Queue;
	struct _RTL_SRWLOCK Lock;
} APCP_QUEUE, * PAPCP_QUEUE;

typedef struct _FULL_APC_POOL
{
	struct _APCP_REFCOUNT Refcount;
	long Padding_239;
	union _APCP_POOL_QUEUE_STATE QueueState;
	struct _APCP_QUEUE* TaskQueue[ 3 ];
	struct _APCP_NUMA_NODE* NumaNode;
	struct _GROUP_AFFINITY* ProximityInfo;
	void* WorkerFactory;
	void* CompletionPort;
	struct _RTL_SRWLOCK Lock;
	struct _LIST_ENTRY PoolObjectList;
	struct _LIST_ENTRY WorkerList;
	struct _APCP_TIMER_QUEUE TimerQueue;
	struct _RTL_SRWLOCK ShutdownLock;
	UINT8 ShutdownInitiated;
	UINT8 Released;
	UINT16 PoolFlags;
	long Padding_240;
	struct _LIST_ENTRY PoolLinks;
	struct _APCP_CALLER AllocCaller;
	struct _APCP_CALLER ReleaseCaller;
	volatile INT32 AvailableWorkerCount;
	volatile INT32 LongRunningWorkerCount;
	UINT32 LasAPCrocCount;
	volatile INT32 NodeStatus;
	volatile INT32 BindingCount;
	UINT32 CallbackChecksDisabled : 1;
	UINT32 TrimTarget : 11;
	UINT32 TrimmedThrdCount : 11;
	UINT32 SelectedCpuSetCount;
	long Padding_241;
	struct _RTL_CONDITION_VARIABLE TrimComplete;
	struct _LIST_ENTRY TrimmedWorkerList;
} FULL_APC_POOL, * PFULL_APC_POOL;

typedef struct _ALPC_WORK_ON_BEHALF_TICKET
{
	UINT32 ThreadId;
	UINT32 ThreadCreationTimeLow;
} ALPC_WORK_ON_BEHALF_TICKET, * PALPC_WORK_ON_BEHALF_TICKET;

typedef union _APCP_WORK_STATE
{
	union
	{
		INT32 Exchange;
		UINT32 Insertable : 1;
		UINT32 PendingCallbackCount : 31;
	};
} APCP_WORK_STATE, * PAPCP_WORK_STATE;

typedef struct _APCP_ITE_WAITER
{
	struct _APCP_ITE_WAITER* Next;
	void* ThreadId;
} APCP_ITE_WAITER, * PAPCP_ITE_WAITER;

typedef struct _APCP_PH_LINKS
{
	struct _LIST_ENTRY Siblings;
	struct _LIST_ENTRY Children;
	INT64 Key;
} APCP_PH_LINKS, * PAPCP_PH_LINKS;

typedef struct _APCP_ITE
{
	struct _APCP_ITE_WAITER* First;
} APCP_ITE, * PAPCP_ITE;

typedef union _APCP_FLAGS_COUNT
{
	union
	{
		UINT64 Count : 60;
		UINT64 Flags : 4;
		INT64 Data;
	};
} APCP_FLAGS_COUNT, * PAPCP_FLAGS_COUNT;

typedef struct _APCP_BARRIER
{
	volatile union _APCP_FLAGS_COUNT Ptr;
	struct _RTL_SRWLOCK WaitLock;
	struct _APCP_ITE WaitList;
} APCP_BARRIER, * PAPCP_BARRIER;

typedef struct _APC_CLEANUP_GROUP
{
	struct _APCP_REFCOUNT Refcount;
	INT32 Released;
	struct _RTL_SRWLOCK MemberLock;
	struct _LIST_ENTRY MemberList;
	struct _APCP_BARRIER Barrier;
	struct _RTL_SRWLOCK CleanupLock;
	struct _LIST_ENTRY CleanupList;
} APC_CLEANUP_GROUP, * PAPC_CLEANUP_GROUP;


typedef struct _APCP_CLEANUP_GROUP_MEMBER
{
	struct _APCP_REFCOUNT Refcount;
	long Padding_233;
	const struct _APCP_CLEANUP_GROUP_MEMBER_VFUNCS* VFuncs;
	struct _APC_CLEANUP_GROUP* CleanupGroup;
	void* CleanupGroupCancelCallback;
	void* FinalizationCallback;
	struct _LIST_ENTRY CleanupGroupMemberLinks;
	struct _APCP_BARRIER CallbackBarrier;
	union
	{
		void* Callback;
		void* WorkCallback;
		void* SimpleCallback;
		void* TimerCallback;
		void* WaitCallback;
		void* IoCallback;
		void* AlpcCallback;
		void* AlpcCallbackEx;
		void* JobCallback;
	};
	void* Context;
	struct _ACTIVATION_CONTEXT* ActivationContext;
	void* SubProcessTag;
	struct _GUID ActivityId;
	struct _ALPC_WORK_ON_BEHALF_TICKET WorkOnBehalfTicket;
	void* RaceDll;
	FULL_APC_POOL* Pool;
	struct _LIST_ENTRY PoolObjectLinks;
	union
	{
		volatile INT32 Flags;
		UINT32 LongFunction : 1;
		UINT32 Persistent : 1;
		UINT32 UnusedPublic : 14;
		UINT32 Released : 1;
		UINT32 CleanupGroupReleased : 1;
		UINT32 InCleanupGroupCleanupList : 1;
		UINT32 UnusedPrivate : 13;
	};
	long Padding_234;
	struct _APCP_CALLER AllocCaller;
	struct _APCP_CALLER ReleaseCaller;
	enum _APC_CALLBACK_PRIORITY CallbackPriority;
	INT32 __PADDING__[ 1 ];
} APCP_CLEANUP_GROUP_MEMBER, * PAPCP_CLEANUP_GROUP_MEMBER;

typedef struct _FULL_APC_WORK
{
	struct _APCP_CLEANUP_GROUP_MEMBER CleanupGroupMember;
	struct _APC_TASK Task;
	volatile union _APCP_WORK_STATE WorkState;
	INT32 __PADDING__[ 1 ];
} FULL_APC_WORK, * PFULL_APC_WORK;

typedef struct _FULL_APC_TIMER
{
	struct _FULL_APC_WORK Work;
	struct _RTL_SRWLOCK Lock;
	union
	{
		struct _APCP_PH_LINKS WindowEndLinks;
		struct _LIST_ENTRY ExpirationLinks;
	};
	struct _APCP_PH_LINKS WindowStartLinks;
	INT64 DueTime;
	struct _APCP_ITE Ite;
	UINT32 Window;
	UINT32 Period;
	UINT8 Inserted;
	UINT8 WaitTimer;
	union
	{
		UINT8 TimerStatus;
		UINT8 InQueue : 1;
		UINT8 Absolute : 1;
		UINT8 Cancelled : 1;
	};
	UINT8 BlockInsert;
	INT32 __PADDING__[ 1 ];
} FULL_APC_TIMER, * PFULL_APC_TIMER;

typedef struct _FULL_APC_WAIT
{
	struct _FULL_APC_TIMER Timer;
	void* Handle;
	void* WaiAPCkt;
	void* NextWaitHandle;
	union _LARGE_INTEGER NextWaitTimeout;
	struct _APC_DIRECT Direct;
	union
	{
		union
		{
			UINT8 AllFlags;
			UINT8 NextWaitActive : 1;
			UINT8 NextTimeoutActive : 1;
			UINT8 CallbackCounted : 1;
			UINT8 Spare : 5;
		};
	} WaitFlags;
	char __PADDING__[ 7 ];
} FULL_APC_WAIT, * PFULL_APC_WAIT;

typedef struct _FULL_APC_IO
{
	struct _APCP_CLEANUP_GROUP_MEMBER CleanupGroupMember;
	struct _APC_DIRECT Direct;
	void* File;
	volatile INT32 PendingIrpCount;
	INT32 __PADDING__[ 1 ];
} FULL_APC_IO, * PFULL_APC_IO;

typedef struct _FULL_APC_ALPC
{
	struct _APC_DIRECT Direct;
	struct _APCP_CLEANUP_GROUP_MEMBER CleanupGroupMember;
	void* AlpcPort;
	INT32 DeferredSendCount;
	INT32 LastConcurrencyCount;
	union
	{
		UINT32 Flags;
		UINT32 ExTypeCallback : 1;
		UINT32 CompletionListRegistered : 1;
		UINT32 Reserved : 30;
	};
	INT32 __PADDING__[ 1 ];
} FULL_APC_ALPC, * PFULL_APC_ALPC;

typedef struct _FULL_APC_JOB
{
	struct _APC_DIRECT Direct;
	struct _APCP_CLEANUP_GROUP_MEMBER CleanupGroupMember;
	void* JobHandle;
	union
	{
		volatile int64_t CompletionState;
		int64_t Rundown : 1;
		int64_t CompletionCount : 63;
	};
	struct _RTL_SRWLOCK RundownLock;
} FULL_APC_JOB, * PFULL_APC_JOB;

typedef VOID( NTAPI* PAPC_ALPC_CALLBACK )(
	_Inout_ PTP_CALLBACK_INSTANCE Instance,
	_Inout_opt_ PVOID Context,
	_In_ PFULL_APC_ALPC Alpc
	);
