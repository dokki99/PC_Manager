USE [PCDB]
GO
/****** Object:  Table [dbo].[Customer_Info]    Script Date: 2024-10-09 오후 6:59:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Customer_Info](
	[C_ID] [varchar](30) NOT NULL,
	[C_Name] [nvarchar](30) NOT NULL,
	[C_PWD] [varchar](30) NOT NULL,
	[C_Phone] [varchar](30) NOT NULL,
	[C_Adress] [nvarchar](50) NOT NULL,
	[C_Birth] [varchar](15) NOT NULL,
	[C_Time] [datetime2](7) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[C_ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Employee_Info]    Script Date: 2024-10-09 오후 6:59:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Employee_Info](
	[E_ID] [varchar](30) NOT NULL,
	[E_Name] [nvarchar](10) NOT NULL,
	[E_PWD] [varchar](20) NULL,
	[E_Permission] [bit] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[E_ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Revenue_Info]    Script Date: 2024-10-09 오후 6:59:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Revenue_Info](
	[R_No] [int] IDENTITY(1,1) NOT NULL,
	[S_Code] [varchar](30) NOT NULL,
	[E_ID] [varchar](30) NOT NULL,
	[R_Date] [datetime2](7) NOT NULL,
	[R_Amount] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[R_No] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Stock_Info]    Script Date: 2024-10-09 오후 6:59:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Stock_Info](
	[S_Code] [varchar](30) NOT NULL,
	[S_Name] [nvarchar](30) NOT NULL,
	[S_Price] [int] NOT NULL,
	[S_Amount] [int] NOT NULL,
	[S_Category] [nvarchar](30) NOT NULL,
	[S_Margin] [decimal](5, 2) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[S_Code] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Work_Info]    Script Date: 2024-10-09 오후 6:59:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Work_Info](
	[W_No] [int] IDENTITY(1,1) NOT NULL,
	[E_ID] [varchar](30) NOT NULL,
	[W_Start] [varchar](30) NOT NULL,
	[W_End] [datetime2](7) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[W_No] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[Revenue_Info]  WITH CHECK ADD  CONSTRAINT [FK_E_ID] FOREIGN KEY([E_ID])
REFERENCES [dbo].[Employee_Info] ([E_ID])
GO
ALTER TABLE [dbo].[Revenue_Info] CHECK CONSTRAINT [FK_E_ID]
GO
ALTER TABLE [dbo].[Revenue_Info]  WITH CHECK ADD  CONSTRAINT [FK_S_Code] FOREIGN KEY([S_Code])
REFERENCES [dbo].[Stock_Info] ([S_Code])
GO
ALTER TABLE [dbo].[Revenue_Info] CHECK CONSTRAINT [FK_S_Code]
GO
ALTER TABLE [dbo].[Work_Info]  WITH CHECK ADD  CONSTRAINT [FK2_E_ID] FOREIGN KEY([E_ID])
REFERENCES [dbo].[Employee_Info] ([E_ID])
GO
ALTER TABLE [dbo].[Work_Info] CHECK CONSTRAINT [FK2_E_ID]
GO
