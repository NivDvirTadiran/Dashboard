# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Aeonix Contact Center Dashboard (Frontend)** - Angular 19 application providing real-time dashboard widgets for contact center monitoring and reporting. Built with GridStack for drag-and-drop dashboard layouts, Chart.js for data visualization, and comprehensive Storybook documentation.

## Key Commands

### Development
```bash
npm start                # Start dev server with proxy (port 4200)
npm run start-8080       # Start with embedded proxy config
npm run start_prod       # Start with production config
npm test                 # Run unit tests with Karma
npm run watch            # Build and watch for changes
```

### Build & Deploy
```bash
npm run build            # Development build → ../Backend/src/main/webapp
npm run build_prod       # Production build → ../Backend/src/main/webapp
```

### Storybook (Component Library)
```bash
npm run storybook        # Start Storybook dev server (port 6006)
npm run build-storybook  # Build static Storybook
```

## Architecture Overview

### Module Structure
- **AppModule**: Main app with lazy-loaded routes to `login2` and `dashboard` modules
- **Login2Module**: Authentication components with Storybook integration
- **DashboardModule**: GridStack-based dashboard with widget system

### Widget System (Core Feature)
Located in `src/stories/sb-gridstack/`:
- **Widget Manager**: `gs-dashboard-widget-manager.service.ts` handles widget lifecycle
- **Base Components**: `sb-gridstack.component` manages the grid layout
- **Widget Types**: 15+ widgets including agents-list, group-list, chart widgets, info widgets
- **Real-time Data**: Widgets use `EmisSoapService` for backend integration with 6-60 second update intervals
- **Configuration**: Widgets use `.nlat` files (100+ templates) for report configurations

### Data Integration
- **EmisSoapService**: Main service for backend SOAP API integration (`src/app/services/emis-soap.service.ts`)
- **Proxy Configuration**: `proxy.conf.json` routes `/accDashboard` to `https://localhost:8445`
- **API Endpoints**: Modernized REST endpoints for agents, groups, DNIS, IVR, and chart data

### Component Library (Storybook)
- **Login Components**: Complete authentication UI in `src/stories/login/`
- **Dashboard Widgets**: Individual widget components with `.stories.ts` files
- **Shared Components**: Translation pipes, UI elements, form controls
- **Design System**: SCSS tokens and variables in `src/stories/assets/scss/`

## Technology Stack
- **Angular 19** with TypeScript 5.6.3
- **GridStack 12.1.2** for dashboard layout
- **Chart.js 4.4.9** for data visualization
- **Angular Material 19** for UI components
- **Bootstrap 5** for styling
- **Storybook 8.5.6** for component documentation
- **Three.js 0.170.0** for 3D graphics

## Development Guidelines

### Code Style
- 2-space indentation, single quotes for TypeScript (per .editorconfig)
- Standalone Angular components architecture
- RxJS observables for data streams and real-time updates
- Comprehensive error handling in service layer

### Widget Development
- Extend from base widget patterns in `sb-gridstack/`
- Use `EmisSoapService` for data fetching
- Implement proper cleanup for real-time subscriptions
- Include corresponding `.stories.ts` file for Storybook

### File Locations
- **Main App**: `src/app/` (routing, modules, core services)
- **Component Library**: `src/stories/` (reusable components with Storybook)
- **Dashboard Widgets**: `src/stories/sb-gridstack/`
- **Login Components**: `src/stories/login/`
- **Services**: `src/app/services/` and `src/app/_services/`

## Backend Integration
- Proxied requests to backend server at `https://localhost:8445/accDashboard`
- SOAP API endpoints under `/auth/api/soap/` path
- Chart endpoints under `/auth/chart` path
- Response format includes `responseInfoHeader` with error handling

## Build Output
Development and production builds output to `../Backend/src/main/webapp` with base href `/accDashboard/` for integration with Java backend.