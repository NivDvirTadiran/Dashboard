import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, catchError, map, throwError, of, delay } from 'rxjs';
import { AppConfig } from "../app.config";

// Define interfaces based on WSDL (ensure these match the actual structure from WSDL/Java types)

export interface RequestInfoHeaderType {
  serviceReqId: number;
  ticket: string;
}

export interface ResponseInfoHeaderType {
  serviceReqId: number;
  errorCause: number;
  serversInfo: string;
}

export interface AgentsListDataItemType {
  agentId: number;
  agentName: string;
  agentNumber: string;
  // Add other fields if present in the WSDL/Java type
}

export interface AgentsListReturnType {
  responseInfoHeader: ResponseInfoHeaderType;
  returnArray: AgentsListDataItemType[];
}

// Interfaces for GroupList
export interface GroupListDataItemType {
  grpId: number;
  grpName: string;
  grpNumber: string;
  grpEmailEnable: number; // Assuming xsd:unsignedInt maps to number
}

export interface GroupListReturnType {
  responseInfoHeader: ResponseInfoHeaderType;
  returnArray: GroupListDataItemType[];
}

// Example for another operation (BriefAgents) to show scalability
export interface BriefInfoRequestType {
  requestInfoHeader: RequestInfoHeaderType;
  // Add other specific fields for BriefInfoRequestType if any
}

export interface DataItemType {
  ersid: string;
  value: string;
  ersname: string;
  bgcolor?: number;
}

export interface DataItemRow {
  returnArray: DataItemType[];
}

export interface ReturnMatrixDataTypeBG {
  responseInfoHeader: ResponseInfoHeaderType;
  returnMatrix: DataItemRow[];
}

// Interfaces for DnisList
export interface DnisListItemType {
  dnisId: number;
  dnisName: string;
}

export interface DnisListReturnType {
  responseInfoHeader: ResponseInfoHeaderType;
  returnArray: DnisListItemType[];
}

// Interface for Detail Response (used by various detail endpoints)
export interface DetailResponseType { // This is for the older SOAP proxy endpoints
  responseInfoHeader: ResponseInfoHeaderType;
  returnArray: DataItemType[];
}

// Interfaces for SuperGroupList (Modernized API)
export interface SuperGroupListItemType {
  superGroupId: number;
  superGroupName: string;
  returnArray: GroupListDataItemType[]; // Nested list of groups
}

export interface SuperGroupListReturnType {
  responseInfoHeader: ResponseInfoHeaderType;
  returnArray: SuperGroupListItemType[];
}

// Interfaces for Modernized Detail Responses (e.g., groupAbandonedInfo)
export interface BlockItemType {
  id: number;
  name: string;
  returnArray: DataItemType[];
}

export interface ModernizedDetailResponseType {
  responseInfoHeader: ResponseInfoHeaderType;
  returnArrayOfBlocks: BlockItemType[];
}

export interface GroupDetailInfoDataItemType {
  id: number;
  name: string;
  returnArray: DataItemType[]; // Nested list of groups
}

export interface GroupDetailInfoResponseType {
  responseInfoHeader: ResponseInfoHeaderType;
  returnArrayOfBlocks: GroupDetailInfoDataItemType[];
}

// Interface for Chart parameters
export interface ChartRequestParams {
  type?: string;
  title?: string;
  keys?: string;
  size?: string;
  refObjectName?: string;
  // Add any other parameters the chart endpoint might need
}

// Interfaces for Chart Data API
export interface ChartDataset {
  label: string;
  data: number[];
  backgroundColor: string[];
  borderColor?: string[];
  borderWidth?: number;
}

export interface ChartDataResponse {
  labels: string[];
  datasets: ChartDataset[];
}

// Add DTO for DNIS list items
export interface DnisListItemDto {
  id: string; // Or number, depending on your data model
  name: string;
  number: string; // Actual DNIS number
  type?: number;  // from dnisType (unsignedInt)
}

// Define and export AgentListItemDto
export interface AgentListItemDto {
  id: string; // from agentId (number)
  name: string; // from agentName (string)
  number?: string; // from agentNumber (string), optional
}

@Injectable({
  providedIn: 'root'
})
export class EmisSoapService {
  // The base path for these proxied SOAP calls should align with TemplateController's @RequestMapping
  // If TemplateController is under "/auth", and new endpoints are "/api/soap/...",
  // then the full path is like "/auth/api/soap/OperationName"
  private basePath = AppConfig.accServer.ACCWEBServers + '/accDashboard/auth/api/soap'; // For existing SOAP proxied calls
  private chartBasePath = AppConfig.accServer.ACCWEBServers + '/accDashboard/auth'; // For non-SOAP calls like chart

  constructor(private http: HttpClient) { }

  getAgentsList(): Observable<AgentsListDataItemType[]> {
    return this.http.get<AgentsListReturnType>(`${this.basePath}/AgentsList`).pipe(
      map(response => {
        if (response && response.returnArray) {
          return response.returnArray;
        }
        // Optional: Handle cases where returnArray might be null or undefined,
        // or where responseInfoHeader indicates an error that wasn't caught by HTTP status codes.
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in AgentsList:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
          // Depending on requirements, you might throw an error here or return an empty array
        }
        return []; // Default to empty array if no data or specific error handling
      })
    );
  }

  getGroupList(): Observable<GroupListDataItemType[]> {
    return this.http.get<GroupListReturnType>(`${this.basePath}/GroupList`).pipe(
      map(response => {
        if (response && response.returnArray) {
          return response.returnArray;
        }
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in GroupList:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
        }
        return [];
      })
    );
  }

  // Example for BriefAgents - define parameters if needed
  getBriefAgents(agentIds?: string[]): Observable<ReturnMatrixDataTypeBG> {
    let requestUrl = `${this.basePath}/brief-agents`;
    if (agentIds && agentIds.length > 0) {
      const agentIdsParam = agentIds.join(',');
      requestUrl += `?agentIds=${agentIdsParam}`; // Ensure backend expects 'agentIds'
    }
    // For now, assuming it's a GET. If POST is needed for complex params, this would change.
    return this.http.get<ReturnMatrixDataTypeBG>(requestUrl).pipe(
      map(response => {
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in getBriefAgents:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
          // Depending on requirements, you might throw an error here or return a modified response
          // For now, let the component handle the error display based on the full response.
        }
        return response; // Return the full response object
      }),
      catchError(err => {
        console.error('HTTP error fetching BriefAgents:', err);
        return throwError(() => new Error(`Failed to fetch BriefAgents data: ${err.message || 'Server error'}`));
      })
    );
  }

  // New method for Brief DNIS
  getBriefDnis(dnisIds?: string[]): Observable<ReturnMatrixDataTypeBG> {
    let requestUrl = `${this.basePath}/brief-dnis`;
    if (dnisIds && dnisIds.length > 0) {
      const dnisIdsParam = dnisIds.join(',');
      requestUrl += `?dnisIds=${dnisIdsParam}`; // Ensure backend expects 'dnisIds'
    }
    return this.http.get<ReturnMatrixDataTypeBG>(requestUrl).pipe(
      map(response => {
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in getBriefDnis:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
        }
        return response; // Return the full response object
      }),
      catchError(err => {
        console.error('HTTP error fetching BriefDnis:', err);
        return throwError(() => new Error(`Failed to fetch BriefDnis data: ${err.message || 'Server error'}`));
      })
    );
  }

  getDnisList(): Observable<DnisListItemType[]> {
    return this.http.get<DnisListReturnType>(`${this.basePath}/dnis-list`).pipe(
      map(response => {
        if (response && response.returnArray) {
          return response.returnArray;
        }
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in DnisList:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
        }
        return [];
      })
    );
  }

  getGroupDetailFullInfo(groupId: number): Observable<GroupDetailInfoDataItemType[]> {
    return this.http.get<GroupDetailInfoResponseType>(`${this.basePath}/group/${groupId}/detail-full`).pipe(
      map(response => {
        if (response && response.returnArrayOfBlocks) {
          return response.returnArrayOfBlocks;
        }
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in getGroupDetailFullInfo:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
        }
        return [];
      })
    );
  }

  getGroupQueInfo(groupId: number): Observable<GroupDetailInfoDataItemType[]> {
    return this.http.get<GroupDetailInfoResponseType>(`${this.basePath}/group/${groupId}/queue`).pipe(
      map(response => {
        if (response && response.returnArrayOfBlocks) {
          return response.returnArrayOfBlocks;
        }
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in getGroupQueInfo:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
        }
        return [];
      })
    );
  }

  getGroupAgentsInfo(groupId: number): Observable<DataItemType[]> {
    return this.http.get<DetailResponseType>(`${this.basePath}/group/${groupId}/agents-info`).pipe(
      map(response => {
        if (response && response.returnArray) {
          return response.returnArray;
        }
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in getGroupAgentsInfo:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
        }
        return [];
      })
    );
  }

  getIvrApplicationInfo(appId: number): Observable<DataItemType[]> {
    return this.http.get<DetailResponseType>(`${this.basePath}/ivr/application/${appId}`).pipe(
      map(response => {
        if (response && response.returnArray) {
          return response.returnArray;
        }
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in getIvrApplicationInfo:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
        }
        return [];
      })
    );
  }

  getIvrPortInfo(setting: any): Observable<DataItemType[]> {
    return this.http.get<DetailResponseType>(`${this.basePath}/ivr/port/${setting}`).pipe(
      map(response => {
        if (response && response.returnArray) {
          return response.returnArray;
        }
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP service error in getIvrPortInfo:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
        }
        return [];
      })
    );
  }

  // New methods for Modernized API
  getSuperGroupList(): Observable<SuperGroupListReturnType> { // Return the full DTO
    return this.http.get<SuperGroupListReturnType>(`${this.basePath}/super-groups`).pipe(
      map(response => {
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP API error in getSuperGroupList:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
          // Optionally throw an error or return a modified response
        }
        return response; // Return the full response object
      })
    );
  }

  getGroupAbandonedInfo(groupId: number): Observable<ModernizedDetailResponseType> {
    return this.http.get<ModernizedDetailResponseType>(`${this.basePath}/group/${groupId}/abandoned-info`).pipe(
      map(response => {
        if (response && response.responseInfoHeader && response.responseInfoHeader.errorCause !== 0) {
          console.error('SOAP API error in getGroupAbandonedInfo:', response.responseInfoHeader.errorCause, response.responseInfoHeader.serversInfo);
          // Optionally throw an error or return a modified response
        }
        return response; // Return the full response object
      })
    );
  }
  // Add other soap API calls here as needed...

  getChart(params: ChartRequestParams): Observable<Blob> {
    return this.http.get(`${this.chartBasePath}/chart`, {
      params: { ...params }, // Spread parameters into HttpParams
      responseType: 'blob'   // Expect a Blob response (image data)
    }).pipe(
      map(response => {
        if (!response) {
          console.error('Chart data response is null or undefined');
          throw new Error('Failed to load chart: No data received');
        }
        return response;
      }),
      catchError(err => {
        console.error('Error fetching chart data in EmisSoapService:', err);
        return throwError(() => new Error(`Failed to load chart: ${err.message || 'Server error'}`));
      })
    );
  }

  getChartData(params: ChartRequestParams): Observable<ChartDataResponse> {
    return this.http.get<ChartDataResponse>(`${this.chartBasePath}/chart-data`, {
      params: { ...params } // Spread parameters into HttpParams
    }).pipe(
      map(response => {
        if (!response) {
          console.error('Chart data response is null or undefined');
          throw new Error('Failed to load chart data: No data received');
        }
        return response;
      }),
      catchError(err => {
        console.error('Error fetching chart data in EmisSoapService:', err);
        return throwError(() => new Error(`Failed to load chart data: ${err.message || 'Server error'}`));
      })
    );
  }
}
